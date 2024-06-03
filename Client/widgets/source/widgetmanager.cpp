//
// Created by User on 19.05.2024.
//

#include "../header/widgetmanager.h"

void WidgetManager::setButtonIcon(QPushButton *tempButton, const QString& iconPath) {
    auto *movie = tempButton->findChild<QMovie*>();

    if (!movie) {
        movie = new QMovie(tempButton);
        movie->setFileName(iconPath);

        QObject::connect(movie, &QMovie::frameChanged, [=] {
            if (movie->currentPixmap().isNull()) {
                qWarning(logWarning()) << "Icon gif is not found!";
                movie->stop();
            }
            if (movie->currentFrameNumber() == movie->frameCount() - 1) movie->stop();
            tempButton->setIcon(movie->currentPixmap());
        });
        movie->jumpToNextFrame();
        return;
    }

    movie->start();
}
