#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioDevice>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMediaCaptureSession>

class QCamera;
class QGraphicsVideoItem;
class VideoScene;
class QPermission;
class QListWidgetItem;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_refreshButton_clicked();
#ifdef USE_PERMISSIONS
    void permissionsChanged(const QPermission& permission);
#endif
    void updateCameraList();
    void loadCamera(const QCameraDevice& device);

    void on_camListWidget_itemSelectionChanged();

private:
    void printCameraFormat(const QCameraFormat& format);

    Ui::MainWindow* ui;
    VideoScene* m_scene;
    QGraphicsVideoItem* m_videoItem;

    QMediaDevices m_devices;
    QList<QCameraDevice> m_availableCams;
    QList<QAudioDevice> m_availableMics;
    QScopedPointer<QCamera> m_camera;
    //QMediaCaptureSession m_captureSession;
};
#endif // MAINWINDOW_H
