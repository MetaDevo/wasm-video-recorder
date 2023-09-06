#ifdef USE_PERMISSIONS
#include <QPermissions>
#endif

#include <QAudioDevice>
#include <QCamera>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsVideoItem>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "VideoScene.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef USE_PERMISSIONS
    //QCameraPermission camPermission;
    switch (qApp->checkPermission(QCameraPermission{})) {
    case Qt::PermissionStatus::Undetermined:
        qDebug() << "Undetermined";
        qApp->requestPermission(QCameraPermission{}, this, &MainWindow::permissionsChanged);
        break;
    case Qt::PermissionStatus::Denied:
        qDebug() << "Denied";
        break;
    case Qt::PermissionStatus::Granted:
        qDebug() << "Granted";
        updateCameraList();
        break;
    }
#else
    updateCameraList();
    connect(&m_devices, &QMediaDevices::videoInputsChanged, this, &MainWindow::updateCameraList);
#endif

    m_videoItem = new QGraphicsVideoItem();
    m_videoItem->setSize(QSizeF(1280, 720));
    m_videoItem->setAspectRatioMode(Qt::KeepAspectRatio);

    m_scene = new VideoScene(this);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->scene()->addItem(m_videoItem);
    ui->graphicsView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

#ifdef USE_PERMISSIONS
void MainWindow::permissionsChanged(const QPermission& permission)
{
    if (permission.status() == Qt::PermissionStatus::Granted) {
        updateCameraList();
        connect(&m_devices, &QMediaDevices::videoInputsChanged, this, &MainWindow::updateCameraList);
    } else {
        qDebug() << "Permission not granted.";
    }
}
#endif

void MainWindow::updateCameraList()
{
    m_availableCams = QMediaDevices::videoInputs();
    //m_availableMics = QMediaDevices::audioInputs();
    qDebug() << Q_FUNC_INFO << "Num cams: " << m_availableCams.size();
    //qDebug() << Q_FUNC_INFO << "Num mics: " << m_availableMics.size();

    ui->camListWidget->clear();
    int i = 0;
    for (const QCameraDevice& cameraDevice : m_availableCams) {
        qDebug() << "Cam " << ++i << cameraDevice.description();
        QString desc = cameraDevice.description();
        if (desc.isEmpty()) {
            qDebug() << "ID: " << cameraDevice.id();
            desc = "camera " + QString::number(i);
            if (cameraDevice.isNull()) {
                qDebug() << desc << "is null.";
            }
        }
        ui->camListWidget->addItem(desc);
    }
}

void MainWindow::loadCamera(const QCameraDevice& device)
{
    m_camera.reset(new QCamera(device));
    //m_captureSession.setCamera(m_camera.data());
    //m_captureSession.setVideoOutput(m_videoItem);

    qDebug() << Q_FUNC_INFO << "Initial format:";
    printCameraFormat(m_camera->cameraFormat());

    if (m_camera->cameraFormat().isNull()) {
        qDebug() << Q_FUNC_INFO << "Setting format...";

        auto formats = device.videoFormats();
        if (!formats.isEmpty())
        {
            QCameraFormat bestFormat;
            for (const auto &fmt : formats)
            {
                if (bestFormat.maxFrameRate() < 23 && fmt.maxFrameRate() > bestFormat.maxFrameRate()) {
                    bestFormat = fmt;
                } else if (bestFormat.maxFrameRate() == fmt.maxFrameRate() &&
                         bestFormat.resolution().width() * bestFormat.resolution().height() <
                             fmt.resolution().width() * fmt.resolution().height()) {
                    bestFormat = fmt;
                }
            }
            m_camera->setCameraFormat(bestFormat);
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "No formats available.";
        }
    }


    //m_mediaRecorder->setVideoFrameRate(bestFormat.maxFrameRate());

    qDebug() << Q_FUNC_INFO << "Final format:";
    printCameraFormat(m_camera->cameraFormat());

    m_camera->start();
}

void MainWindow::printCameraFormat(const QCameraFormat& format)
{
    qDebug() << Q_FUNC_INFO << "Pixel format: " << m_camera->cameraFormat().pixelFormat();
    qDebug() << Q_FUNC_INFO << "Max Framerate: " << m_camera->cameraFormat().maxFrameRate();
    qDebug() << Q_FUNC_INFO << "Min Framerate: " << m_camera->cameraFormat().minFrameRate();
    qDebug() << Q_FUNC_INFO << "Resolution: " << m_camera->cameraFormat().resolution();
}

void MainWindow::on_refreshButton_clicked()
{
    updateCameraList();
}

void MainWindow::on_camListWidget_itemSelectionChanged()
{
    int currentRow = ui->camListWidget->currentRow();
    if (currentRow > -1) {
        qDebug() << "Loading cam " << currentRow + 1;
        loadCamera(m_availableCams.at(currentRow));
    }
}

