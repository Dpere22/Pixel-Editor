#include "model.h"

/// Source code style checked by Diego Perez u1301598
const int MAX_UNDO_STACK = 10;

Model::Model(QObject *parent)
    : QObject(parent)
    , currentAnimationFrame(0)
    , currentAnimationSpeed(1000)
    , currentFrame(0)
    , currentColor(0, 0, 0)
    , hasBeenSaved(false)
    , isSaving(false)
    , wantsToSave(true)
    , pixelsHeight(32)
    , pixelsWidth(32)
{
    connect(&animationTimer, &QTimer::timeout, this, [this] {
        if (currentAnimationFrame < frames.size() - 1) {
            currentAnimationFrame++;
        } else {
            currentAnimationFrame = 0;
        }
        emit animationFrameSent(currentAnimationFrame);
    });
}

void Model::setAnimationInterval(int interval)
{
    currentAnimationSpeed = 1000 / interval;
    if (animationTimer.isActive()) {
        animationTimer.setInterval(currentAnimationSpeed);
    }
}

void Model::toggleTimer()
{
    if (animationTimer.isActive()) {
        animationTimer.stop();
    } else {
        animationTimer.start(currentAnimationSpeed);
    }
    emit timerToggled(animationTimer.isActive());
}

void Model::finishSetup(int width, int height)
{
    pixelsWidth = width;
    pixelsHeight = height;
    addFrame();
}

void Model::updatePixel(const QPoint &point, const QRect &window, const QColor &color)
{
    int pixelX = point.x() * pixelsWidth / window.width();
    int pixelY = point.y() * pixelsHeight / window.height();
    if (pixelX < 0 || pixelY < 0 || pixelX >= pixelsWidth || pixelY >= pixelsHeight) {
        return;
    }
    frames[currentFrame][pixelY][pixelX] = color;
    sendFrameUpdate(currentFrame);
}

int Model::getCurrentFrameIndex()
{
    return currentFrame;
}

void Model::updateCurrentFrame(int index)
{
    redoStack.clear();
    undoStack.clear();
    sendToUndoStack(frames.at(currentFrame));
    currentFrame = index;
    emit currentFrameUpdated(index);
}

void Model::copyFrame()
{
    PixelFrame frame(frames.at(currentFrame));
    if (currentFrame == frames.size() - 1 || frames.size() == 0) {
        frames.push_back(frame);
        currentFrame = frames.size() - 1;
    } else {
        frames.insert(currentFrame + 1, frame);
        currentFrame = currentFrame + 1;
    }
    emit frameAmountUpdated(frames.size());
    sendFrameUpdate(currentFrame);
    updateCurrentFrame(currentFrame);
}

void Model::deleteFrame()
{
    if (frames.size() < 2) {
        return;
    }
    frames.takeAt(currentFrame);
    if (currentFrame > 0) {
        currentFrame--;
    }
    emit frameAmountUpdated(frames.size());
    sendFrameUpdate(currentFrame);
    updateCurrentFrame(currentFrame);
}

void Model::addFrame()
{

    PixelFrame frame(pixelsHeight, QVector<QColor>(pixelsWidth, Qt::transparent));
    if (currentFrame == frames.size() - 1 || frames.size() == 0) {
        frames.push_back(frame);
        currentFrame = frames.size() - 1;
    } else {
        frames.insert(currentFrame + 1, frame);
        currentFrame = currentFrame + 1;
    }
    emit frameAmountUpdated(frames.size());
    sendFrameUpdate(currentFrame);
    updateCurrentFrame(currentFrame);
}

void Model::undo()
{
    if (undoStack.size() < 2) {
        return;
    }
    PixelFrame current = undoStack.takeLast();
    redoStack.push_back(current);
    frames[currentFrame] = undoStack.last();
    sendFrameUpdate(currentFrame);
}

void Model::clearFrame()
{
    PixelFrame emptyFrame(pixelsHeight, QVector<QColor>(pixelsWidth, Qt::transparent));
    sendToUndoStack(frames[currentFrame]);
    frames[currentFrame] = emptyFrame;
    sendFrameUpdate(currentFrame);
}

void Model::redo()
{
    if (redoStack.isEmpty()) {
        return;
    }
    PixelFrame current = redoStack.takeLast();
    undoStack.push_back(current);
    frames[currentFrame] = undoStack.last();
    sendFrameUpdate(currentFrame);
}

void Model::sendToUndoStack(const PixelFrame &frame)
{
    if (undoStack.size() > MAX_UNDO_STACK) {
        undoStack.pop_front();
    }
    PixelFrame frameCopy(frame);
    undoStack.push_back(frameCopy);
    hasBeenSaved = false;
    redoStack.clear();
}

void Model::changeColor(const QColor &color)
{
    currentColor = color;
}

void Model::sendFrameUpdate(int index)
{
    if (frames.size() <= index) {
        return;
    }
    PixelFrame updated = frames.at(index);
    emit frameUpdated(index, updated);
}

void Model::doesntWantSave(){
    wantsToSave = false;
}

void Model::storePixel(const QPoint &point, const QRect &window)
{
    updatePixel(point, window, currentColor);
}

void Model::removePixel(const QPoint &point, const QRect &window)
{
    updatePixel(point, window, Qt::transparent);
}

void Model::resizePixels(int height, int width)
{
    pixelsHeight = height;
    pixelsWidth = width;
}

void Model::saveToJson(const QString &filename)
{
    isSaving = true;
    if(filename != ""){currFileName = filename;}
    QJsonObject root;
    QJsonArray jsonFrames;
    for (const auto &frame : frames) {
        QJsonArray currentFrame;
        for(const auto &row: frame){
            QJsonArray currentRow;
            for(const auto &color: row){
                QJsonArray currentColor;
                currentColor.push_back(color.red());
                currentColor.push_back(color.green());
                currentColor.push_back(color.blue());
                currentColor.push_back(color.alpha());
                currentRow.push_back(currentColor);
            }
            currentFrame.push_back(currentRow);
        }
        jsonFrames.push_back(currentFrame);
    }
    root["frames"] = jsonFrames;
    QByteArray byteArray;
    byteArray = QJsonDocument(root).toJson();
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)){
        isSaving = false;
        return;
    }
    file.write(byteArray);
    file.close();

    hasBeenSaved = true;
    isSaving = false;
}

void Model::exportToPNG(const QString &filename){
    // Build a QImage using the current image data, and use that to export to a png.
    PixelFrame currFrame = frames[currentFrame];
    int rows = currFrame.size();
    int columns = currFrame[0].size();
    QImage image(rows, columns, QImage::Format_ARGB32);

    // For each row in the frame
    for(int x = 0; x < rows; x++){
        // For each pixel in the row, get its color, and use image.setPixelColor
        // to make the corrseponding pixel in the QImage match the color from the frame.
        for(int y = 0; y < columns; y++){
            image.setPixelColor(x, y, currFrame[y][x]);
        }
    }

    // When you're done looping, save the qimage to a png.
    image.save(filename, nullptr, -1);
}

void Model::loadFromJson(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() <<"file not found";
        return;
    }
    QByteArray byteArray;
    byteArray = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return;
    }

    QVector<PixelFrame> tempFrames; //will replace frames with this
    QJsonObject root;
    root = jsonDoc.object();
    QJsonArray jsonFrames(root["frames"].toArray());
    //to array here converts to a QJsonArray not to a primitive array
    for(int i = 0; i < jsonFrames.size(); i++){ //go through each frame
        QJsonArray jsonFrame(jsonFrames[i].toArray());
        PixelFrame tempFrame;
        for(int i = 0; i < jsonFrame.size(); i++){ //go through each row in the frame
            QJsonArray jsonRow(jsonFrame[i].toArray());
            QVector<QColor> tempRow;
            for(int i = 0; i < jsonRow.size(); i++){//get the color values for each color
                QJsonArray tempColor(jsonRow[i].toArray());
                QColor qColor(tempColor[0].toInt(),tempColor[1].toInt(),tempColor[2].toInt(),tempColor[3].toInt());
                tempRow.push_back(qColor);
            }
            tempFrame.push_back(tempRow);
        }
        tempFrames.push_back(tempFrame);
    }
    frames = tempFrames;
    animationTimer.stop();
    emit timerToggled(false);
    undoStack.clear();
    redoStack.clear();
    currentFrame = 0;
    pixelsWidth = frames[0][0].size();
    pixelsHeight = frames[0].size();
    emit frameUpdated(0, frames[0]); //display the first frame to the screen
    emit frameAmountUpdated(frames.size());
    updateCurrentFrame(0);
}

void Model::openNew(){
    // Wait until isSaving is no longer true.
    while(isSaving == true){}

    if(hasBeenSaved == false && wantsToSave == true)
    {
        emit checkSave();
        return;
    }

    currFileName = "";
    frames.clear();
    addFrame();

    sendFrameUpdate(0);
    wantsToSave = true;
}
