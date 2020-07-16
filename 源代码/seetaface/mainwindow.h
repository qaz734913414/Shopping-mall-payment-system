#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <map>
#include <QMainWindow>
#include <QItemSelectionModel>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QModelIndex>
#include <QHBoxLayout>
#include <QWidget>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QDateTime>
#include <QTimer>
#include <QSqlRecord>
#include <QCompleter>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QItemSelectionModel>
#include <chrono>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStandardItemModel>

#include "videocapturethread.h"
#include "g.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getdatas(QString sql);
    void cleardata();

    //令程序暂停m毫秒
    void mwsleep(int m);
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    /********tab0 主页********/

    void on_Find_Id_pushButton_clicked();

    void on_Goods_tableView_doubleClicked(const QModelIndex &index);

    void on_Pay_pushButton_clicked();

    void on_menuopencamera_clicked();

    void on_logout_pushButton_clicked();

    void on_tmp_del_all_btn_clicked();

    void showtime_slot();

    /********tab1 会员信息********/

    void on_dbsavebtn_clicked();

    void on_dbtableview_doubleClicked(const QModelIndex &index);

    void on_lineEdit_textChanged(QString);

    /********tab2 预览********/

    void on_previewclearbtn_clicked();

    void on_previewrunbtn_clicked();

    void on_previewstopbtn_clicked();

    void on_previewsavebtn_clicked();

    void showfaceinfo();



    /********tab3 商品管理********/

    void GoodsModelDataChanged(QModelIndex, QModelIndex);

    void on_query_lineEdit_textChanged(QString );

    void on_add_goods_btn_clicked();

    void on_edit_cancle_btn_clicked();

    void on_edit_save_btn_clicked();

    void on_del_goods_btn_clicked();


    /********tab4 销售记录********/

    void on_query_pushButton_clicked();

    void on_history_tableView_clicked(const QModelIndex &index);


    /********tab5 会员规则设置********/

    void on_add_vip_btn_clicked();

    void on_del_vip_btn_clicked();

    void on_edit_vip_save_btn_clicked();

    void on_edit_vip_cancle_btn_clicked();


    /********tab6 识别参数设置********/

    void on_settingsavebtn_clicked();

    /********切换tab页时触发********/
    void on_tabWidget_currentChanged(int index);

    /********更新预览页显示图像********/
    void onupdateui(const QImage & image,const QImage & image2);

    /********对预览页的人脸记录进行数量调整********/
    void onrecognize(int pid, const QString & name, const QString & imagepath, float score, const QImage &image, const QRect &rc);

    /********根据传入value值判断摄像头运行状况********/
    void onvideothreadend(int value);

private:
    Ui::MainWindow *ui;

    QSqlTableModel *FaceManage;
    QSqlTableModel *VIPManage;
    QSqlTableModel *GoodsManage;
    QSqlTableModel *SealsRecord;
    QSqlTableModel *SingleRecord;

    QItemSelectionModel *theSelection; //选择模型
    QItemSelectionModel *theVIPSelection; //选择模型
    QTimer *timer;

    QSqlTableModel *tabModel;
    VideoCaptureThread * m_videothread;

    QString          m_table;//人脸表
    QString          m_config_table;//人脸识别参数表
    QStandardItemModel * m_model;

    QPixmap          m_default_image;

    QString          m_image_path;
    QString          m_pics_path;

    std::map<int, DataInfo *> m_datalst;

    int              m_currenttab;

    QStandardItemModel * m_videomodel;

    RecognizeType        m_type;


};

#endif // MAINWINDOW_H
