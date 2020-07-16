#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std::chrono;

const QString gcrop_prefix("crop_");
Config_Paramter gparamters;
std::string gmodelpath;
QImage ime;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_currenttab = -1;
    ui->setupUi(this);
    qDebug() << Operator;



    /***************创建一个定时器，用来显示系统当前的时间*************/
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showtime_slot()));
    timer->start(1000);

    ui->operator_label->setText(Operator);

    QIntValidator * vfdminfacesize = new QIntValidator(20, 1000);
    ui->fdminfacesize->setValidator(vfdminfacesize);

    QDoubleValidator *vfdthreshold = new QDoubleValidator(0.0,1.0, 2);
    ui->fdthreshold->setValidator(vfdthreshold);

    QDoubleValidator *vantispoofclarity = new QDoubleValidator(0.0,1.0, 2);
    ui->antispoofclarity->setValidator(vantispoofclarity);

    QDoubleValidator *vantispoofreality = new QDoubleValidator(0.0,1.0, 2);
    ui->antispoofreality->setValidator(vantispoofreality);

    QDoubleValidator *vyawhigh = new QDoubleValidator(0.0,90, 2);
    ui->yawhighthreshold->setValidator(vyawhigh);

    QDoubleValidator *vyawlow = new QDoubleValidator(0.0,90, 2);
    ui->yawlowthreshold->setValidator(vyawlow);

    QDoubleValidator *vpitchlow = new QDoubleValidator(0.0,90, 2);
    ui->pitchlowthreshold->setValidator(vpitchlow);

    QDoubleValidator *vpitchhigh = new QDoubleValidator(0.0,90, 2);
    ui->pitchhighthreshold->setValidator(vpitchhigh);

    QDoubleValidator *vfrthreshold = new QDoubleValidator(0.0,1.0, 2);
    ui->fr_threshold->setValidator(vfrthreshold);

    gparamters.MinFaceSize = 100;
    gparamters.Fd_Threshold = 0.80;
    gparamters.VideoWidth = 400;
    gparamters.VideoHeight = 400;
    gparamters.AntiSpoofClarity = 0.30;
    gparamters.AntiSpoofReality = 0.80;
    gparamters.PitchLowThreshold = 20;
    gparamters.PitchHighThreshold = 10;
    gparamters.YawLowThreshold = 20;
    gparamters.YawHighThreshold = 10;
    gparamters.Fr_Threshold = 0.6;

    m_type.type = 0;
    m_type.filename = "";
    m_type.title = "Open Camera 0";

    ui->recognize_label->setText(m_type.title);

    int width = this->width();
    int height = this->height();
    this->setFixedSize(width, height);

    ui->db_editpicture->setStyleSheet("border-image:url(:/new/prefix1/default.png)");
    ui->db_editcrop->setStyleSheet("border-image:url(:/new/prefix1/default.png)");

    QString exepath = QCoreApplication::applicationDirPath();

    m_image_path = exepath + "/images/";
    m_pics_path = exepath + "/pics/";

    gmodelpath = (exepath + "/models/").toStdString();

    QDir dir;
    dir.mkpath(m_image_path);
    dir.mkpath(m_pics_path);

    m_table = "face_tab";
    m_config_table = "setting_tab";

    ui->fdminfacesize->setText(QString::number(gparamters.MinFaceSize));
    ui->fdthreshold->setText(QString::number(gparamters.Fd_Threshold));
    ui->antispoofclarity->setText(QString::number(gparamters.AntiSpoofClarity));
    ui->antispoofreality->setText(QString::number(gparamters.AntiSpoofReality));
    ui->yawlowthreshold->setText(QString::number(gparamters.YawLowThreshold));
    ui->yawhighthreshold->setText(QString::number(gparamters.YawHighThreshold));
    ui->pitchlowthreshold->setText(QString::number(gparamters.PitchLowThreshold));
    ui->pitchhighthreshold->setText(QString::number(gparamters.PitchHighThreshold));
    ui->fr_threshold->setText(QString::number(gparamters.Fr_Threshold));


    ui->dbtableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dbtableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dbtableview->verticalHeader()->setDefaultSectionSize(80);
    ui->dbtableview->verticalHeader()->hide();


    m_model = new QStandardItemModel(this);
    QStringList columsTitles;
    columsTitles << "ID" << "姓名" << "照片" << "会员等级" << "会员积分";
    m_model->setHorizontalHeaderLabels(columsTitles);
    ui->dbtableview->setModel(m_model);
    ui->dbtableview->setColumnWidth(0, 120);
    ui->dbtableview->setColumnWidth(1, 200);
    ui->dbtableview->setColumnWidth(2, 104);

    QString sql = "select * from " + m_table + " order by id  asc";
    getdatas(sql);
    gparamters.VideoWidth = ui->previewlabel->width();
    gparamters.VideoHeight = ui->previewlabel->height();

    //fd_minfacesize, fd_threshold, antispoof_clarity, antispoof_reality, qa_yawlow, qa_yawhigh, qa_pitchlow, qa_pitchhigh
    QSqlQuery q("select * from " + m_config_table);
    while(q.next())
    {
        gparamters.MinFaceSize = q.value("fd_minfacesize").toInt();
        ui->fdminfacesize->setText(QString::number(q.value("fd_minfacesize").toInt()));

        gparamters.Fd_Threshold = q.value("fd_threshold").toFloat();
        ui->fdthreshold->setText(QString::number(q.value("fd_threshold").toFloat()));

        gparamters.AntiSpoofClarity = q.value("antispoof_clarity").toFloat();
        ui->antispoofclarity->setText(QString::number(q.value("antispoof_clarity").toFloat()));

        gparamters.AntiSpoofReality = q.value("antispoof_reality").toFloat();
        ui->antispoofreality->setText(QString::number(q.value("antispoof_reality").toFloat()));

        gparamters.YawLowThreshold = q.value("qa_yawlow").toFloat();
        ui->yawlowthreshold ->setText(QString::number(q.value("qa_yawlow").toFloat()));

        gparamters.YawHighThreshold = q.value("qa_yawhigh").toFloat();
        ui->yawhighthreshold ->setText(QString::number(q.value("qa_yawhigh").toFloat()));

        gparamters.PitchLowThreshold = q.value("qa_pitchlow").toFloat();
        ui->pitchlowthreshold ->setText(QString::number(q.value("qa_pitchlow").toFloat()));

        gparamters.PitchHighThreshold = q.value("qa_pitchhigh").toFloat();
        ui->pitchhighthreshold ->setText(QString::number(q.value("qa_pitchhigh").toFloat()));

        gparamters.Fr_Threshold = q.value("fr_threshold").toFloat();

        ui->fr_threshold->setText(QString::number(gparamters.Fr_Threshold));
    }

    ui->previewtableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->previewtableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->previewtableview->verticalHeader()->setDefaultSectionSize(80);
    ui->previewtableview->verticalHeader()->hide();

    m_videomodel = new QStandardItemModel(this);
    columsTitles.clear();
    columsTitles  << "姓名" << "相似度" << "会员库" << "快照" << "PID";
    m_videomodel->setHorizontalHeaderLabels(columsTitles);
    ui->previewtableview->setModel(m_videomodel);
    ui->previewtableview->setColumnWidth(0, 100);
    ui->previewtableview->setColumnWidth(1, 80);
    ui->previewtableview->setColumnWidth(2, 84);
    ui->previewtableview->setColumnWidth(3, 84);
    //ui->previewtableview->setColumnWidth(4, 2);
    ui->previewtableview->setColumnHidden(4,true);

    m_videothread = new VideoCaptureThread(&m_datalst, ui->previewlabel->width(), ui->previewlabel->height());
    m_videothread->setparamter();

    ui->dbsavebtn->setEnabled(true);
    ui->previewrunbtn->setEnabled(true);
    ui->previewstopbtn->setEnabled(false);

    int a = ui->previewlabel->width();
    int b = ui->previewlabel->height();
    QImage image(":/new/prefix1/white.png");
    QImage ime2 = image.scaled(a,b);
    ui->previewlabel->setPixmap(QPixmap::fromImage(ime2));

    ui->tabWidget->setCurrentIndex(0);
    m_currenttab = ui->tabWidget->currentIndex();

    //qDebug() << "m_model->rowCount():" << m_model->rowCount();
    if(m_model->rowCount() > 0)
    {
        ui->dbtableview->scrollToBottom();
        ui->dbtableview->selectRow(m_model->rowCount() - 1);
        emit ui->dbtableview->clicked(m_model->index(m_model->rowCount() - 1, 1));
    }

    tabModel = new QSqlTableModel(this);
    tabModel->setTable("Tmps");
    ui->Goods_tableView->setModel(tabModel);
    ui->Goods_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Goods_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->Goods_tableView->setColumnHidden(0,true);
    tabModel->setHeaderData(1,Qt::Horizontal,"商品名字");
    tabModel->setHeaderData(2,Qt::Horizontal,"商品价格");
    tabModel->setHeaderData(3,Qt::Horizontal,"商品单位");
    tabModel->setHeaderData(4,Qt::Horizontal,"商品数量");
    ui->Find_Goods_Id_lineEdit->setFocus();

    GoodsManage = new QSqlTableModel(this);
    GoodsManage->setTable("Goods");
    GoodsManage->setEditStrategy(QSqlTableModel::OnManualSubmit);

    SealsRecord = new QSqlTableModel(this);
    SealsRecord->setTable("History");
    SealsRecord->setHeaderData(SealsRecord->fieldIndex("time"),Qt::Horizontal,"购买时间");
    SealsRecord->setHeaderData(SealsRecord->fieldIndex("old_price"),Qt::Horizontal,"原价");
    SealsRecord->setHeaderData(SealsRecord->fieldIndex("new_price"),Qt::Horizontal,"折扣价");
    SealsRecord->setHeaderData(SealsRecord->fieldIndex("buyer"),Qt::Horizontal,"购买者");
    SealsRecord->setHeaderData(SealsRecord->fieldIndex("operator"),Qt::Horizontal,"操作员");

    ui->history_tableView->setModel(SealsRecord);
    ui->history_tableView->setColumnWidth(1,150);
    ui->history_tableView->setColumnHidden(SealsRecord->fieldIndex("id"),true);
    ui->history_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->history_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    SingleRecord = new QSqlTableModel(this);
    SingleRecord->setTable("Seals");
    SingleRecord->setHeaderData(SingleRecord->fieldIndex("Seal_Name"),Qt::Horizontal,"商品名字");
    SingleRecord->setHeaderData(SingleRecord->fieldIndex("Seal_Price"),Qt::Horizontal,"商品价格");
    SingleRecord->setHeaderData(SingleRecord->fieldIndex("Seal_Num"),Qt::Horizontal,"购买数量");
    SingleRecord->setHeaderData(SingleRecord->fieldIndex("Seal_Unit"),Qt::Horizontal,"商品单位");
    ui->history_tableView_2->setModel(SingleRecord);
    ui->history_tableView_2->setColumnHidden(SingleRecord->fieldIndex("Seal_Id"),true);
    ui->history_tableView_2->setColumnHidden(SingleRecord->fieldIndex("Seal_Time"),true);
    ui->history_tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->history_tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    VIPManage = new QSqlTableModel(this);
    VIPManage->setTable("VIP");
    VIPManage->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->VIP_tableView->setModel(VIPManage);

    VIPManage->setHeaderData(VIPManage->fieldIndex("vip_rank"),Qt::Horizontal,"VIP等级");
    VIPManage->setHeaderData(VIPManage->fieldIndex("vip_discount"),Qt::Horizontal,"折扣");
    VIPManage->setHeaderData(VIPManage->fieldIndex("vip_points"),Qt::Horizontal,"升级积分");
    VIPManage->setHeaderData(VIPManage->fieldIndex("vip_money_ratio"),Qt::Horizontal,"金钱积分比");

    FaceManage = new QSqlTableModel(this);
    FaceManage->setTable("face_tab");

    if(Operator != "admin")
    {
        ui->tabWidget->setTabEnabled(3,false);
        ui->tabWidget->setTabEnabled(5,false);
    }
    connect(ui->dbtableview, SIGNAL(clicked(QModelIndex)), this, SLOT(showfaceinfo()));
    connect(m_videothread, SIGNAL(sigUpdateUI(const QImage &,const QImage &)), this, SLOT(onupdateui(const QImage &,const QImage &)));
    connect(m_videothread, SIGNAL(sigEnd(int)), this, SLOT(onvideothreadend(int)));
    connect(m_videothread->m_workthread, SIGNAL(sigRecognize(int, const QString &, const QString &, float, const QImage &, const QRect &)), this,
            SLOT(onrecognize(int, const QString &, const QString &, float, const QImage &, const QRect &)));
    connect(GoodsManage, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(GoodsModelDataChanged(QModelIndex,QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
    cleardata();
}

void MainWindow::cleardata()
{
    std::map<int, DataInfo *>::iterator iter = m_datalst.begin();
    for(; iter != m_datalst.end(); ++iter)
    {
        if(iter->second)
        {
            delete iter->second;
            iter->second = NULL;
        }
    }
    m_datalst.clear();
}

void MainWindow::getdatas(QString sql)
{
      int i = 0;
      QSqlQuery q(sql);
      while(q.next())
      {
          QByteArray data1 = q.value("feature_data").toByteArray();
          float * ptr = (float *)data1.data();
          m_model->setItem(i, 0, new QStandardItem(QString::number(q.value("id").toInt())));
          m_model->setItem(i, 1, new QStandardItem(q.value("name").toString()));

          ui->dbtableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

          QLabel *label = new QLabel("");
          label->setFixedSize(100,80);
          label->setStyleSheet("border-image:url(" + m_image_path + q.value("image_path").toString() + ")");
          ui->dbtableview->setIndexWidget(m_model->index(m_model->rowCount() - 1, 2), label);

          ui->dbtableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

          m_model->setItem(i, 3, new QStandardItem(q.value("vip_rank").toString()));
          m_model->setItem(i, 4, new QStandardItem(q.value("vip_points").toString()));

          DataInfo * info = new DataInfo;
          info->id = q.value("id").toInt();
          info->name = q.value("name").toString();
          info->image_path = q.value("image_path").toString();
          memcpy(info->features, ptr, 1024 * sizeof(float));
          info->x = q.value("facex").toInt();
          info->y = q.value("facey").toInt();
          info->width = q.value("facewidth").toInt();
          info->height = q.value("faceheight").toInt();
          info->vip_rank = q.value("vip_rank").toString();
          info->vip_points = q.value("vip_points").toString();
          m_datalst.insert(std::map<int, DataInfo *>::value_type(info->id, info));
          i++;
      }
}

void MainWindow::showfaceinfo()
{
    int row = ui->dbtableview->currentIndex().row();

    if(row >= 0)
    {
        QModelIndex index = m_model->index(row, 0);
        int id = ui->db_editid->text().toInt();
        int curid = m_model->data(index).toInt();
        if(id == curid)
            return;


        ui->db_editid->setText(QString::number(m_model->data(index).toInt()));
        std::map<int, DataInfo *>::iterator iter = m_datalst.find(m_model->data(index).toInt());
        if(iter == m_datalst.end())
            return;

        index = m_model->index(row, 1);
        ui->db_editname->setText(m_model->data(index).toString());

        QString strimage = iter->second->image_path;
        //qDebug() << "showfaceinfo:" << strimage;
        ui->db_editpicture->setStyleSheet("border-image:url(" + m_image_path + strimage + ")");


        //qDebug() << "showfaceinfo:" << strimage;
        ui->db_editcrop->setStyleSheet("border-image:url(" + m_image_path + gcrop_prefix + strimage + ")");


        iter = m_datalst.find(id);
        if(iter == m_datalst.end())
            return;

    }
}

void MainWindow::onvideothreadend(int value)
{
    qDebug() << "onvideothreadend:" << value;
    //ui->label->setStyleSheet("border-image:url(:/new/prefix1/white.png)");

    if(m_type.type != 2)
    {
        int a = ui->previewlabel->width();
        int b = ui->previewlabel->height();
        QImage image(":/new/prefix1/white.png");
        QImage ime1 = image.scaled(a,b);
        ui->previewlabel->setPixmap(QPixmap::fromImage(ime1));
        ui->previewlabel->show();
    }

    ui->previewrunbtn->setEnabled(true);
    ui->previewstopbtn->setEnabled(false);
}

void MainWindow::onrecognize(int pid, const QString & name, const QString & imagepath, float score, const QImage &image, const QRect &rc)
{

    //qDebug() << "name:" << name;

    int nrows = m_videomodel->rowCount();

    if(nrows > 1000)
    {
        ui->previewtableview->setUpdatesEnabled(false);
        m_videomodel->removeRows(0, 200);
        ui->previewtableview->setUpdatesEnabled(true);
    }

    nrows = m_videomodel->rowCount();
    int i = 0;
    for(; i<nrows; i++)
    {
        if(m_videomodel->item(i, 4)->text().toInt() == pid)
        {
            break;
        }
    }

    nrows = i;

    m_videomodel->setItem(nrows, 0, new QStandardItem(name));
    //m_videomodel->setItem(nrows, 1, new QStandardItem(QString::number(score, 'f', 3)));

    QLabel *label = new QLabel("");
    label->setFixedSize(80,80);
    if(name.isEmpty())
    {
        m_videomodel->setItem(nrows, 1, new QStandardItem(""));
        label->setText(imagepath);
    }else
    {
        m_videomodel->setItem(nrows, 1, new QStandardItem(QString::number(score, 'f', 3)));

        QImage srcimage;
        srcimage.load( m_image_path + imagepath);
        srcimage = srcimage.copy(rc.x(),rc.y(),rc.width(),rc.height());
        srcimage = srcimage.scaled(80,80);
        label->setPixmap(QPixmap::fromImage(srcimage));
     }

    ui->previewtableview->setIndexWidget(m_videomodel->index(nrows, 2), label);

    QLabel *label2 = new QLabel("");
    label2->setFixedSize(80,80);
    QImage img = image.scaled(80,80);
    label2->setPixmap(QPixmap::fromImage(img));
    ui->previewtableview->setIndexWidget(m_videomodel->index(nrows, 3), label2);

    m_videomodel->setItem(nrows, 4, new QStandardItem(QString::number(pid)));
    ui->previewtableview->scrollToBottom();

    if(Old_Price==0 && New_Price==0)
    {
        ui->previewstopbtn->click();
        QMessageBox::warning(NULL, "警告", tr("没有买东西!"), QMessageBox::Yes);
        ui->tabWidget->setCurrentIndex(0);
    }
    else if(Old_Price!=New_Price)
    {
        //qDebug() << "Old_Price:" << Old_Price;
        //qDebug() << "New_Price:" << New_Price;
        int nIndex1 = imagepath.indexOf(".jpg");
        QString tmp_1 = imagepath.mid(0,nIndex1);
        int nIndex2 = tmp_1.indexOf("_");
        QString tmp_2 = tmp_1.mid(0,nIndex2+1);
        Buyer = tmp_2 + name;
        //qDebug() << "Buyer:" << Buyer;

        int nindex = Buyer.indexOf("_");
        int Id = Buyer.mid(0,nindex).toInt();

        QSqlTableModel *face_model = new QSqlTableModel(this);
        face_model->setTable(m_table);
        face_model->setFilter(tr("id = '%1'").arg(Id));
        face_model->select();

        QModelIndex vip_r = face_model->index(0,face_model->fieldIndex("vip_rank"));
        QString Vip_Rank = face_model->data(vip_r).toString();

        QSqlTableModel *vip_model = new QSqlTableModel(this);
        vip_model->setTable("VIP");
        vip_model->setFilter(tr("vip_rank = '%1'").arg(Vip_Rank));
        vip_model->select();

        //折扣
        QModelIndex vip_discount = vip_model->index(0,vip_model->fieldIndex("vip_discount"));
        float Vip_Discount = vip_model->data(vip_discount).toFloat();

        qDebug() << "折扣：" << Vip_Discount;
        if(New_Price != Old_Price * Vip_Discount)
        {
            QMessageBox::information(NULL, "信息", tr("会员识别成功!"), QMessageBox::Yes);
        }
        New_Price = Old_Price * Vip_Discount;

        ui->old_price_label->setText("原价：￥"+QString::number(Old_Price));
        ui->new_price_label->setText("现价：￥"+QString::number(New_Price));
        ui->tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::onupdateui(const QImage & image,const QImage & image2)
{
    int a = ui->previewlabel->width();
    int b = ui->previewlabel->height();
    ime = image2.scaled(a,b);
    QImage tmp = image.scaled(a,b);
    ui->previewlabel->setPixmap(QPixmap::fromImage(tmp));
    ui->previewlabel->show();
}

//改名
void MainWindow::on_dbsavebtn_clicked()
{
    //qDebug() << "----begin---update";
    if(ui->db_editname->text().isEmpty())
    {
        QMessageBox::critical(NULL, "错误", tr("名字为空!"), QMessageBox::Yes);
        return;
    }

    if(ui->db_editname->text().length() > 64)
    {
        QMessageBox::critical(NULL, "错误", tr("名字长度超过64!"), QMessageBox::Yes);
        return;
    }
    int index = 1;
    index = ui->db_editid->text().toInt();

    //qDebug() << "----begin---update---index:" << index;
    std::map<int, DataInfo *>::iterator iter = m_datalst.find(index);
    if(iter == m_datalst.end())
    {
        return;
    }

    QString tt1 = iter->second->image_path;
    int nIndex = tt1.indexOf(".jpg");
    QString old_image = tt1.mid(0,nIndex);

    QSqlQuery query;
    query.prepare("update " + m_table + " set name = :name,image_path = :image_path where id=" + QString::number(index));

    QString new_name = ui->db_editname->text();
    QString buyer_edit = QString::number(index)+"_"+new_name;
    QString imm = buyer_edit + ".jpg";
    query.bindValue(":name", ui->db_editname->text());
    query.bindValue(":image_path", imm);
    if(!query.exec())
    {
        QMessageBox::critical(NULL, "错误", tr("更新数据失败!"), QMessageBox::Yes);
        return;
    }

    iter->second->name = ui->db_editname->text();

    int row = ui->dbtableview->currentIndex().row();
    QString image_name="";
    //qDebug() << "showfaceinfo:" << row ;
    if(row >= 0)
    {
        QModelIndex index = m_model->index(row, 1);
        image_name = m_model->data(index).toString();

        m_model->itemFromIndex(index)->setText(ui->db_editname->text());
    }
    //qDebug() << "buyer:" << old_image;
    QSqlTableModel *his_model = new QSqlTableModel(this);
    his_model->setTable("History");
    his_model->setFilter(tr("buyer = '%1'").arg(old_image));
    his_model->select();
    int nr = his_model->rowCount();
    //qDebug() << "nr:" << nr;
    for(int i=0; i<nr; i++)
    {
        his_model->setData(his_model->index(i,his_model->fieldIndex("buyer")),buyer_edit);
        his_model->submitAll();
    }

    QFile::rename(m_image_path+"crop_"+ui->db_editid->text()+"_"+image_name+".jpg",m_image_path+"crop_"+ui->db_editid->text()+"_"+new_name+".jpg");
    QFile::rename(m_image_path+ui->db_editid->text()+"_"+image_name+".jpg",m_image_path+ui->db_editid->text()+"_"+new_name+".jpg");
    QMessageBox::information(NULL, "信息", tr("名字修改成功!"), QMessageBox::Yes);
}

void MainWindow::on_previewrunbtn_clicked()
{
    m_videothread->m_exited = false;
    m_videothread->start(m_type);
    ui->previewrunbtn->setEnabled(false);
    ui->previewstopbtn->setEnabled(true);
}

void MainWindow::on_previewstopbtn_clicked()
{
    m_videothread->m_exited = true;
}

void MainWindow::on_settingsavebtn_clicked()
{

    int size = ui->fdminfacesize->text().toInt();
    if(size < 20 || size > 1000)
    {
        QMessageBox::warning(NULL, "warn", "Face Detector Min Face Size is invalid!", QMessageBox::Yes);
        return;
    }

    float value = ui->fdthreshold->text().toFloat();
    if(value >= 1.0 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Face Detector Threshold is invalid!", QMessageBox::Yes);
        return;
    }

    value = ui->antispoofclarity->text().toFloat();
    if(value >= 1.0 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Anti Spoofing Clarity is invalid!", QMessageBox::Yes);
        return;
    }

    value = ui->antispoofreality->text().toFloat();
    if(value >= 1.0 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Anti Spoofing Reality is invalid!", QMessageBox::Yes);
        return;
    }

    value = ui->yawlowthreshold->text().toFloat();
    if(value >= 90 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Quality Yaw Low Threshold is invalid!", QMessageBox::Yes);
        return;
    }
    value = ui->yawhighthreshold->text().toFloat();
    if(value >= 90 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Quality Yaw High Threshold is invalid!", QMessageBox::Yes);
        return;
    }

    value = ui->pitchlowthreshold->text().toFloat();
    if(value >= 90 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Quality Pitch Low Threshold is invalid!", QMessageBox::Yes);
        return;
    }
    value = ui->pitchhighthreshold->text().toFloat();
    if(value >= 90 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Quality Pitch High Threshold is invalid!", QMessageBox::Yes);
        return;
    }

    value = ui->fr_threshold->text().toFloat();
    if(value >= 1.0 || value < 0.0)
    {
        QMessageBox::warning(NULL, "warn", "Face Recognizer Threshold is invalid!", QMessageBox::Yes);
        return;
    }

    QString sql("update " + m_config_table + " set fd_minfacesize=%1, fd_threshold=%2, antispoof_clarity=%3, antispoof_reality=%4,");
    sql += "qa_yawlow=%5, qa_yawhigh=%6, qa_pitchlow=%7, qa_pitchhigh=%8, fr_threshold=%9";
    sql = QString(sql).arg(ui->fdminfacesize->text()).arg(ui->fdthreshold->text()).arg(ui->antispoofclarity->text()).arg(ui->antispoofreality->text()).
            arg(ui->yawlowthreshold->text()).arg(ui->yawhighthreshold->text()).arg(ui->pitchlowthreshold->text()).arg(ui->pitchhighthreshold->text()).
            arg(ui->fr_threshold->text());
    QSqlQuery q(sql);
    if(!q.exec())
    {
        QMessageBox::critical(NULL, "错误", "更新设置失败!", QMessageBox::Yes);
        return;
    }



    gparamters.MinFaceSize = ui->fdminfacesize->text().toInt();
    gparamters.Fd_Threshold = ui->fdthreshold->text().toFloat();
    gparamters.AntiSpoofClarity = ui->antispoofclarity->text().toFloat();
    gparamters.AntiSpoofReality = ui->antispoofreality->text().toFloat();
    gparamters.YawLowThreshold = ui->yawlowthreshold->text().toFloat();
    gparamters.YawHighThreshold = ui->yawhighthreshold->text().toFloat();
    gparamters.PitchLowThreshold = ui->pitchlowthreshold->text().toFloat();
    gparamters.PitchHighThreshold = ui->pitchhighthreshold->text().toFloat();
    gparamters.Fr_Threshold = ui->fr_threshold->text().toFloat();

    m_videothread->setparamter();

    QMessageBox::information(NULL, "信息", "更新设置成功!", QMessageBox::Yes);
}

void MainWindow::on_previewclearbtn_clicked()
{
    ui->previewtableview->setUpdatesEnabled(false);
    m_videomodel->removeRows(0, m_videomodel->rowCount());
    ui->previewtableview->setUpdatesEnabled(true);
}

//人脸识别
void MainWindow::on_menuopencamera_clicked()
{
    m_type.type = 0;
    m_type.filename = "";
    m_type.title = "Open Camera: 0";
    ui->recognize_label->setText(m_type.title);
    ui->tabWidget->setCurrentIndex(2);
    emit ui->previewrunbtn->clicked();
}

static void FindFile(const QString & path, QStringList &files)
{
    QDir dir(path);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst);;

    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    while(i < list.size())
    {
        QFileInfo info = list.at(i);
        //qDebug() << info.absoluteFilePath();
        if(info.isDir())
        {
            FindFile(info.absoluteFilePath(), files);
        }else
        {
            QString str = info.suffix();
            if(str.compare("png", Qt::CaseInsensitive) == 0 || str.compare("jpg", Qt::CaseInsensitive) == 0 || str.compare("jpeg", Qt::CaseSensitive) == 0 || str.compare("bmp", Qt::CaseInsensitive) == 0)
            {
                files.append(info.absoluteFilePath());
            }
        }
        i++;
    }
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_videothread->m_exited = true;
    m_videothread->wait();
    QWidget::closeEvent(event);
}

//会员删除
void MainWindow::on_dbtableview_doubleClicked(const QModelIndex &index)
{
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "删除", tr("你确定删除该记录?"), QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return;

    QStandardItemModel * model = (QStandardItemModel *)ui->dbtableview->model();
    QModelIndex Iindex = model->index(index.row(),0);
    int id=model->data(Iindex).toInt();

    QModelIndex dir_name = model->index(index.row(),1);
    QString str_name =model->data(dir_name).toString();

    QFile filename2(m_image_path+"crop_"+QString::number(id)+"_"+str_name+".jpg");
    filename2.remove();
    QFile filename3(m_image_path+QString::number(id)+"_"+str_name+".jpg");
    filename3.remove();
    QSqlQuery query("delete from " + m_table + " where id=" + QString::number(id));
    if(!query.exec())
    {
        QMessageBox::warning(NULL, "警告", tr("删除记录失败!"), QMessageBox::Yes);
        return;
    }

    int nrows = model->rowCount();
    model->removeRow(index.row());
    std::map<int, DataInfo *>::iterator iter = m_datalst.find(id);
    if(iter != m_datalst.end())
    {
        QFile file(m_image_path + iter->second->image_path);
        file.remove();
        delete iter->second;
        m_datalst.erase(iter);
    }

    if(m_model->rowCount() > 0)
    {
        nrows--;
        if(nrows < 0)
        {
            nrows = 0;
        }
        ui->dbtableview->selectRow(nrows);
        emit ui->dbtableview->clicked(m_model->index(nrows, 1));
    }else
    {
        ui->db_editname->setText("");
        ui->db_editid->setText("");
        ui->db_editpicture->setStyleSheet("border-image:url(:/new/prefix1/default.png)");
        ui->db_editcrop->setStyleSheet("border-image:url(:/new/prefix1/default.png)");
    }
    New_Price = Old_Price;
    ui->old_price_label->setText("原价：￥"+QString::number(Old_Price));
    ui->new_price_label->setText("现价：￥"+QString::number(New_Price));
}

//会员注册
void MainWindow::on_previewsavebtn_clicked()
{
    if(ui->previewrunbtn->isEnabled()==true)
    {
        QMessageBox::critical(NULL, "错误", tr("没有打开摄像头!"), QMessageBox::Yes);
        return ;
    }

    ui->previewstopbtn->click();
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);
    QString fileName = m_pics_path+timestamp+".jpg";
    ime.save(fileName);//保存图片
    mwsleep(500);
    QImage image(fileName);
    if(image.isNull())
        return;

    QFile file(fileName);
    QFileInfo fileinfo(fileName);

    //为注册的会员选择最低等级会员
    QSqlTableModel *VipModel = new QSqlTableModel(this);
    VipModel->setTable("VIP");
    VipModel->select();
    QModelIndex vip1 = VipModel->index(0,VipModel->fieldIndex("vip_rank"));

    QString vip_rank = VipModel->data(vip1).toString();
    float vip_points = 0;

    qDebug() << "等级：" << vip_rank;

    QSqlTableModel *dbModel = new QSqlTableModel(this);
    dbModel->setTable("face_tab");

    dbModel->select();
    int rowCout = dbModel->rowCount();
    dbModel->insertRow(rowCout,QModelIndex());
    dbModel->setData(dbModel->index(rowCout,1),"123");
    dbModel->submitAll();

    dbModel->select();
    QModelIndex curIndex=dbModel->index(dbModel->rowCount()-1,0);
    int index = dbModel->data(curIndex).toInt();

    QString strfile = QString::number(index) + "_" + fileinfo.fileName();
    QString cropfile = m_image_path + gcrop_prefix + strfile;

    float features[1024];
    SeetaRect rect;
    int nret = m_videothread->checkimage(fileName, cropfile, features, rect);
    QString strerror;
    if(nret == -2)
    {
        strerror = "没找到人脸!";
    }else if(nret == -1)
    {
        strerror = fileName + " 是无效的!";
    }else if(nret == 1)
    {
        strerror = "发现多张人脸!";
    }else if(nret == 2)
    {
        strerror = "质量检测失败!";
    }

    if(!strerror.isEmpty())
    {
        QMessageBox::critical(NULL,"错误", strerror, QMessageBox::Yes);
        QFile file(fileName);
        file.remove();
        return;
    }

    QString name = fileinfo.completeBaseName();
    int n = name.indexOf("_");
    if(n >= 1)
    {
        name = name.left(n);
    }

    QSqlQuery query;
    query.prepare("update " + m_table + " set name = :name,image_path = :image_path,feature_data = :feature_data,facex = :facex,facey = :facey,facewidth = :facewidth,faceheight = :faceheight,vip_rank = :vip_rank,vip_points = :vip_points where id=" + QString::number(index));

    query.bindValue(0,name);
    query.bindValue(1, strfile);

    QByteArray bytearray;
    bytearray.resize(1024 * sizeof(float));
    memcpy(bytearray.data(), features, 1024 * sizeof(float));

    query.bindValue(2, QVariant(bytearray));
    query.bindValue(3, rect.x);
    query.bindValue(4, rect.y);
    query.bindValue(5, rect.width);
    query.bindValue(6, rect.height);
    query.bindValue(7, vip_rank);
    query.bindValue(8, QString::number(vip_points));

    if(!query.exec())
    {
         QFile::remove(cropfile);
         qDebug() << "failed to insert table:" << query.lastError();
         //QMessageBox::critical(NULL, "错误", tr("保存人脸到数据库失败!"), QMessageBox::Yes);
         return;
    }
    file.copy(m_image_path + strfile);

    DataInfo * info = new DataInfo();
    info->id = index;
    info->name = name;
    info->image_path = strfile;
    memcpy(info->features, features, 1024 * sizeof(float));
    info->x = rect.x;
    info->y = rect.y;
    info->width = rect.width;
    info->height = rect.height;
    info->vip_rank = vip_rank;
    info->vip_points = QString::number(vip_points);
    m_datalst.insert(std::map<int, DataInfo *>::value_type(index, info));

    int rows = m_model->rowCount();
    //qDebug() << "rows:" << rows;

    m_model->setItem(rows, 0, new QStandardItem(QString::number(index)));
    m_model->setItem(rows, 1, new QStandardItem(info->name));
    m_model->setItem(rows, 3, new QStandardItem(info->vip_rank));
    m_model->setItem(rows, 4, new QStandardItem(info->vip_points));

    QLabel *label = new QLabel("");

    label->setStyleSheet("border-image:url(" + m_image_path + strfile + ")");
    ui->dbtableview->setIndexWidget(m_model->index(rows, 2), label);

    ui->dbtableview->scrollToBottom();
    ui->dbtableview->selectRow(rows);

    file.remove();
    QMessageBox::information(NULL, "信息", tr("会员注册成功!"), QMessageBox::Yes);
    emit ui->dbtableview->clicked(m_model->index(rows, 1));
    ui->previewclearbtn->click();

}

//购买商品
void MainWindow::on_Find_Id_pushButton_clicked()
{ 
    if(ui->Find_Goods_Id_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, "警告", tr("输入为空!"), QMessageBox::Yes);
        return;
    }
    int Goods_Id = ui->Find_Goods_Id_lineEdit->text().toInt();
    QSqlTableModel * t_Model = new QSqlTableModel(this);
    t_Model->setTable("Goods");
    t_Model->setFilter(tr("Goods_Id = '%1'").arg(Goods_Id));
    t_Model->select();

    QModelIndex Goods_Name = t_Model->index(0,t_Model->fieldIndex("Goods_Name"));
    QString Name = t_Model->data(Goods_Name).toString();

    if(Name.isEmpty())
    {
        QMessageBox::warning(NULL, "警告", tr("商品不存在!"), QMessageBox::Yes);
        return;
    }

    QModelIndex Goods_Units = t_Model->index(0,t_Model->fieldIndex("Goods_Units"));
    QString Units = t_Model->data(Goods_Units).toString();

    QModelIndex Goods_Price = t_Model->index(0,t_Model->fieldIndex("Goods_Price"));
    float Price = t_Model->data(Goods_Price).toFloat();

    QSqlTableModel * Model2 = new QSqlTableModel(this);
    Model2->setTable("Tmps");
    Model2->setFilter(tr("Tmp_Name = '%1'").arg(Name));
    Model2->select();
    if(Model2->rowCount()<1)
    {
        tabModel->insertRow(tabModel->rowCount());
        tabModel->setData(tabModel->index(tabModel->rowCount()-1,tabModel->fieldIndex("Tmp_Name")),Name);
        tabModel->setData(tabModel->index(tabModel->rowCount()-1,tabModel->fieldIndex("Tmp_Unit")),Units);
        tabModel->setData(tabModel->index(tabModel->rowCount()-1,tabModel->fieldIndex("Tmp_Price")),QString::number(Price));
        tabModel->setData(tabModel->index(tabModel->rowCount()-1,tabModel->fieldIndex("Tmp_Num")),1);
    }
    else
    {
        QModelIndex Tmp_Num = Model2->index(0,Model2->fieldIndex("Tmp_Num"));
        int Num = Model2->data(Tmp_Num).toInt();
        Model2->setData(Model2->index(Model2->rowCount()-1,Model2->fieldIndex("Tmp_Name")),Name);
        Model2->setData(Model2->index(Model2->rowCount()-1,Model2->fieldIndex("Tmp_Unit")),Units);
        Model2->setData(Model2->index(Model2->rowCount()-1,Model2->fieldIndex("Tmp_Price")),QString::number(Price));
        Model2->setData(Model2->index(Model2->rowCount()-1,Model2->fieldIndex("Tmp_Num")),++Num);
        Model2->submitAll();
    }

    // 开始事务操作
    tabModel->database().transaction();
    if (tabModel->submitAll())
    {
        tabModel->database().commit(); //提交
        Old_Price += Price;
        New_Price = Old_Price;
        ui->old_price_label->setText("原价：￥"+QString::number(Old_Price));
        ui->new_price_label->setText("现价：￥"+QString::number(New_Price));
    }
    else
    {
        tabModel->database().rollback(); //回滚
    }
    tabModel->select();


}

//删除商品
void MainWindow::on_Goods_tableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "删除", tr("确定删除该条记录?"), QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::No)
        return;
    int curRow = ui->Goods_tableView->currentIndex().row();
    tabModel->removeRow(curRow);
    tabModel->select();

}

//结算
void MainWindow::on_Pay_pushButton_clicked()
{
    if(tabModel->rowCount()<1)
    {
        QMessageBox::critical(NULL, "错误", tr("没有商品！"), QMessageBox::Yes);
        return;
    }
    //qDebug() << "结算成功:" << Buyer;
    QMessageBox::information(NULL, "信息", tr("结算成功！"), QMessageBox::Yes);
    ui->Find_Goods_Id_lineEdit->setText("");
    ui->old_price_label->setText("原价：￥ 0.00");
    ui->new_price_label->setText("现价：￥ 0.00");

    QSqlTableModel * Model3 = new QSqlTableModel(this);
    Model3->setTable("Tmps");
    Model3->select();

    QSqlTableModel *tmp_Model = new QSqlTableModel(this);
    tmp_Model->setTable("Seals");

    QDateTime  time = QDateTime::currentDateTime();
    QString str_time= time.toString("yyyy-MM-dd hh:mm:ss");
    for(int i=0;i<Model3->rowCount();i++)
    {
        QModelIndex tmp_name = Model3->index(i,Model3->fieldIndex("Tmp_Name"));
        QString Name = Model3->data(tmp_name).toString();

        QModelIndex tmp_price = Model3->index(i,Model3->fieldIndex("Tmp_Price"));
        float Price = Model3->data(tmp_price).toFloat();

        QModelIndex tmp_unit = Model3->index(i,Model3->fieldIndex("Tmp_Unit"));
        QString Unit = Model3->data(tmp_unit).toString();

        QModelIndex tmp_num = Model3->index(i,Model3->fieldIndex("Tmp_Num"));
        int Num = Model3->data(tmp_num).toInt();

        tmp_Model->insertRow(tmp_Model->rowCount());
        tmp_Model->setData(tmp_Model->index(tmp_Model->rowCount()-1,tmp_Model->fieldIndex("Seal_Time")),str_time);
        tmp_Model->setData(tmp_Model->index(tmp_Model->rowCount()-1,tmp_Model->fieldIndex("Seal_Name")),Name);
        tmp_Model->setData(tmp_Model->index(tmp_Model->rowCount()-1,tmp_Model->fieldIndex("Seal_Price")),QString::number(Price));
        tmp_Model->setData(tmp_Model->index(tmp_Model->rowCount()-1,tmp_Model->fieldIndex("Seal_Unit")),Unit);
        tmp_Model->setData(tmp_Model->index(tmp_Model->rowCount()-1,tmp_Model->fieldIndex("Seal_Num")),Num);
        tmp_Model->submitAll();
    }
    QSqlTableModel * Model4 = new QSqlTableModel(this);
    Model4->setTable("History");
    Model4->select();
    Model4->insertRow(Model4->rowCount());
    Model4->setData(Model4->index(Model4->rowCount()-1,Model4->fieldIndex("time")),str_time);
    Model4->setData(Model4->index(Model4->rowCount()-1,Model4->fieldIndex("old_price")),QString::number(Old_Price));
    Model4->setData(Model4->index(Model4->rowCount()-1,Model4->fieldIndex("new_price")),QString::number(New_Price));
    Model4->setData(Model4->index(Model4->rowCount()-1,Model4->fieldIndex("buyer")),Buyer);
    Model4->setData(Model4->index(Model4->rowCount()-1,Model4->fieldIndex("operator")),Operator);
    Model4->submitAll();

    //为会员添加积分，满则升级并提示
    if(Old_Price != New_Price)
    {
        int nindex = Buyer.indexOf("_");
        int Id = Buyer.mid(0,nindex).toInt();
        //qDebug() << Id;

        //获取会员积分和等级
        QSqlTableModel *face_model = new QSqlTableModel(this);
        face_model->setTable(m_table);
        face_model->setFilter(tr("id = '%1'").arg(Id));
        face_model->select();

        QModelIndex vip_r = face_model->index(0,face_model->fieldIndex("vip_rank"));
        QString Vip_Rank = face_model->data(vip_r).toString();

        QModelIndex vip_p = face_model->index(0,face_model->fieldIndex("vip_points"));
        float Vip_Points = face_model->data(vip_p).toFloat();

        //qDebug() << Vip_Rank;
        //qDebug() << Vip_Points;


        //获取VIP晋升信息
        QSqlTableModel *vip_model = new QSqlTableModel();
        vip_model->setTable("VIP");
        vip_model->select();
        int rows = vip_model->rowCount();

        bool is_max = false;
        for(int i=0; i<rows; i++)
        {
            QModelIndex rank = vip_model->index(i,vip_model->fieldIndex("vip_rank"));
            QString Rank = vip_model->data(rank).toString();
            if(Rank == Vip_Rank)
            {
                //升级所需积分数
                QModelIndex need_points = vip_model->index(i,vip_model->fieldIndex("vip_points"));
                float Need_Points = vip_model->data(need_points).toFloat();


                //金钱积分比
                QModelIndex money_ratio = vip_model->index(i,vip_model->fieldIndex("vip_money_ratio"));
                float Money_Ratio = vip_model->data(money_ratio).toFloat();

                //购物后所拥有总积分
                float now_points = New_Price*Money_Ratio + Vip_Points;

                if(now_points < Need_Points)//等级不变，积分累积
                {
                    face_model->setData(face_model->index(0,face_model->fieldIndex("vip_points")),QString::number(now_points));
                    face_model->submitAll();
                    break;
                }

                while(now_points >= Need_Points)
                {
                    if(i+1 >= rows)
                    {
                        face_model->setData(face_model->index(0,face_model->fieldIndex("vip_points")),QString::number(now_points));
                        face_model->submitAll();
                        QMessageBox::information(this, "信息", "会员等级提升到最高了！",QMessageBox::Ok);
                        is_max = true;
                        break;
                    }
                    i++;
                    now_points = now_points - Need_Points;
                    QModelIndex need_points = vip_model->index(i,vip_model->fieldIndex("vip_points"));
                    Need_Points = vip_model->data(need_points).toFloat();
                }

                QModelIndex rank2 = vip_model->index(i,vip_model->fieldIndex("vip_rank"));
                QString Rank2 = vip_model->data(rank2).toString();

                face_model->setData(face_model->index(0,face_model->fieldIndex("vip_rank")),Rank2);
                face_model->setData(face_model->index(0,face_model->fieldIndex("vip_points")),QString::number(now_points));
                face_model->submitAll();
                if(!is_max)
                    QMessageBox::information(this, "信息", "会员等级提升至"+Rank2+"！",QMessageBox::Ok);
                break;
            }
        }
    }
    Old_Price = 0;
    New_Price = 0;
    int rows = tabModel->rowCount();
    for(int i=0;i<rows;i++)
        tabModel->removeRow(i);
    tabModel->select();

    int i = 0;
    QSqlQuery q("select * from " + m_table + " order by id  asc");
    while(q.next())
    {
        m_model->setItem(i, 0, new QStandardItem(QString::number(q.value("id").toInt())));
        m_model->setItem(i, 1, new QStandardItem(q.value("name").toString()));

        QLabel *label = new QLabel("");
        label->setFixedSize(100,80);
        label->setStyleSheet("border-image:url(" + m_image_path + q.value("image_path").toString() + ")");
        ui->dbtableview->setIndexWidget(m_model->index(m_model->rowCount() - 1, 2), label);

        m_model->setItem(i, 3, new QStandardItem(q.value("vip_rank").toString()));
        m_model->setItem(i, 4, new QStandardItem(q.value("vip_points").toString()));
        i++;
    }
    Buyer = "";
}

void MainWindow::mwsleep(int m)
{
    QEventLoop eventloop;
    QTimer::singleShot(m, &eventloop, SLOT(quit()));
    eventloop.exec();
}

void MainWindow::showtime_slot()
{
    QDateTime  time = QDateTime::currentDateTime();
    QString str= time.toString("yyyy-MM-dd hh:mm:ss");
    ui->time_label->setText(str);
}

void MainWindow::on_logout_pushButton_clicked()
{
    qApp->exit(888);//设置重启程序 返回888
}

void MainWindow::on_query_pushButton_clicked()
{
    QString startDateTime = ui->dateEdit_from->text().replace("/","-") + " 00:00:00";
    QString endDateTime = ui->dateEdit_to->text().replace("/","-") + " 23:59:59";
    //qDebug() << startDateTime;
    //qDebug() << endDateTime;
    QString SqlStr = QString("time > '%1' and time < '%2'").arg(startDateTime).arg(endDateTime);
    SealsRecord->setFilter(SqlStr);//按这个方式查询
    SealsRecord->select();
}

void MainWindow::on_add_goods_btn_clicked()
{
    GoodsManage->insertRow(GoodsManage->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=GoodsManage->index(GoodsManage->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行

    int currow=curIndex.row(); //获得当前行
    GoodsManage->setData(GoodsManage->index(currow,0),GoodsManage->rowCount()); //自动生成编号
}

void MainWindow::on_edit_cancle_btn_clicked()
{
    GoodsManage->revertAll();
}

void MainWindow::on_edit_save_btn_clicked()
{
    // 开始事务操作
    GoodsManage->database().transaction();
    if (GoodsManage->submitAll())
    {
        GoodsManage->database().commit(); //提交
        QMessageBox::information(this, "提交成功", "保存成功！",QMessageBox::Ok);
    }
    else
    {
        GoodsManage->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"),tr("数据库错误: %1").arg(GoodsManage->lastError().text()));
    }
}

void MainWindow::on_del_goods_btn_clicked()
{
    //通过tableview去获取被选中的部分的数据model
    QItemSelectionModel * selectModel = ui->goods_manage_tableView->selectionModel();
    //通过选中的数据结构，获取这些格子的ModelIndex
    QModelIndexList selectList =  selectModel->selectedIndexes();
    QList<int> delRow;

    // 遍历这些格子，获取格子所在行，因为可能存在相同的行，所以要去重
    for(int i=0; i<selectList.size(); ++i)
    {
        QModelIndex index = selectList.at(i);
        //GoodsManage->removeRow(index.row());
        delRow << index.row();
    }

    while(delRow.size() > 0)
    {
        int row = delRow.at(0);
        delRow.removeAll(row);
        GoodsManage->removeRow(row);
    }

    int ok = QMessageBox::warning(this,tr("删除选中记录!"),
                                  tr("你确定删除选中记录吗？(不能撤销)"),
                                 QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        GoodsManage->revertAll(); //如果不删除，则撤销
    }
    else GoodsManage->submitAll(); //否则提交，在数据库中删除该行
}

void MainWindow::on_query_lineEdit_textChanged(QString filter)
{
    if(filter.isEmpty())
    {
        GoodsManage->setFilter("");
        GoodsManage->select();
        return;
    }
 //  username like filter or password like  filter .......
    QSqlRecord record = GoodsManage->record();
    QString modelFilter;
    for(int i=0; i<record.count(); ++i)
    {
        if(i!=0)
        {
            modelFilter += " or ";
        }
        QString field = record.fieldName(i);
        QString subFilter = QString().sprintf("%s like '%%%s%%'", field.toUtf8().data(), filter.toUtf8().data());
        //qDebug() << subFilter;

        modelFilter += subFilter;

    }
    //qDebug() << modelFilter;
    GoodsManage->setFilter(modelFilter);
    GoodsManage->select();
}

void MainWindow::GoodsModelDataChanged(QModelIndex,QModelIndex)
{
    QStringList strList;
    for(int i=0; i<GoodsManage->rowCount(); ++i)
    {
        QSqlRecord record = GoodsManage->record(i);
        for(int j=0; j<record.count(); ++j)
        {
            QVariant var = record.value(j);
            if(var.isNull()) continue;
            strList << var.toString();
        }
    }
    //qDebug() << strList;
    QCompleter* completer=new QCompleter(strList);
    ui->query_lineEdit->setCompleter(completer);

}

void MainWindow::on_history_tableView_clicked(const QModelIndex &index)
{
    QModelIndex str_time = SealsRecord->index(index.row(),1);
    QString Time = SealsRecord->data(str_time).toString();
    //qDebug() << "Time" << Time;

    SingleRecord->setFilter(tr("Seal_Time = '%1'").arg(Time));
    SingleRecord->select();

    //qDebug() << "记录条数：" << SingleRecord->rowCount();
}

void MainWindow::on_add_vip_btn_clicked()
{
    VIPManage->insertRow(VIPManage->rowCount(),QModelIndex()); //在末尾添加一个记录

    QModelIndex curIndex=VIPManage->index(VIPManage->rowCount()-1,1);//创建最后一行的ModelIndex
    theVIPSelection->clearSelection();//清空选择项
    theVIPSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行

    int currow=curIndex.row(); //获得当前行
    VIPManage->setData(VIPManage->index(currow,0),VIPManage->rowCount()); //自动生成编号

}

void MainWindow::on_del_vip_btn_clicked()
{
    //通过tableview去获取被选中的部分的数据model
    QItemSelectionModel * selectModel = ui->VIP_tableView->selectionModel();
    //通过选中的数据结构，获取这些格子的ModelIndex
    QModelIndexList selectList =  selectModel->selectedIndexes();
    QList<int> delRow;

    // 遍历这些格子，获取格子所在行，因为可能存在相同的行，所以要去重
    for(int i=0; i<selectList.size(); ++i)
    {
        QModelIndex index = selectList.at(i);
        //VIPManage->removeRow(index.row());
        delRow << index.row();
    }

    while(delRow.size() > 0)
    {
        int row = delRow.at(0);
        delRow.removeAll(row);
        VIPManage->removeRow(row);
    }

    int ok = QMessageBox::warning(this,tr("删除选中记录!"),
                                  tr("你确定删除选中记录吗？(不能撤销)"),
                                 QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        VIPManage->revertAll(); //如果不删除，则撤销
    }
    else VIPManage->submitAll(); //否则提交，在数据库中删除该行
}

void MainWindow::on_edit_vip_save_btn_clicked()
{
    // 开始事务操作
    VIPManage->database().transaction();
    if (VIPManage->submitAll())
    {

        VIPManage->database().commit(); //提交
        QMessageBox::information(this, "提交成功", "保存成功！",QMessageBox::Ok);
    }
    else
    {
        VIPManage->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"),tr("数据库错误: %1").arg(VIPManage->lastError().text()));
    }
}

void MainWindow::on_edit_vip_cancle_btn_clicked()
{
    VIPManage->revertAll();
}

//换页触发函数
void MainWindow::on_tabWidget_currentChanged(int index)
{
    //qDebug() <<  "cur:" << ui->tabWidget->tabText(index) << ",old:" << ui->tabWidget->tabText(m_currenttab) ;
    if(m_currenttab != index)
    {
        if(m_currenttab == 2)
        {
           on_previewstopbtn_clicked();
           m_videothread->wait();
        }
        m_currenttab = index;
    }

    if(m_currenttab==3)//商品管理
    {
        //qDebug() << "商品管理";
        theSelection=new QItemSelectionModel(GoodsManage);//关联选择模型
        GoodsManage->setHeaderData(GoodsManage->fieldIndex("Goods_Id"),Qt::Horizontal,"商品码");
        GoodsManage->setHeaderData(GoodsManage->fieldIndex("Goods_Name"),Qt::Horizontal,"商品名字");
        GoodsManage->setHeaderData(GoodsManage->fieldIndex("Goods_Price"),Qt::Horizontal,"商品价格");
        GoodsManage->setHeaderData(GoodsManage->fieldIndex("Goods_Units"),Qt::Horizontal,"商品单位");

        ui->goods_manage_tableView->setModel(GoodsManage);
        ui->goods_manage_tableView->setColumnHidden(GoodsManage->fieldIndex("Goods_Num"),true);
        ui->goods_manage_tableView->setColumnHidden(GoodsManage->fieldIndex("Goods_Sum"),true);
        GoodsManage->select();
    }
    else if(m_currenttab==4)//销售记录
    {
        SealsRecord->select();
        QDate today = QDate::currentDate();
        ui->dateEdit_from->setCalendarPopup(true);
        ui->dateEdit_from->setDateRange(today.addDays(-90),today);
        ui->dateEdit_to->setCalendarPopup(true);
        ui->dateEdit_to->setDateRange(today.addDays(-90),today);

    }
    else if(m_currenttab==5)//VIP设置
    {
        theVIPSelection=new QItemSelectionModel(GoodsManage);//关联选择模型
        VIPManage->select();
    }

    //qDebug() <<  "tab:" << ui->tabWidget->tabText(index) << ",cur:" << index << ",old:" << ui->tabWidget->currentIndex();
}

//取消订单
void MainWindow::on_tmp_del_all_btn_clicked()
{
    int rows = tabModel->rowCount();
    for(int i=0;i<rows;i++)
        tabModel->removeRow(i);
    tabModel->select();
    Old_Price = 0;
    New_Price = 0;
    ui->Find_Goods_Id_lineEdit->setText("");
    ui->old_price_label->setText("原价：￥ 0.00");
    ui->new_price_label->setText("现价：￥ 0.00");
}


void MainWindow::on_lineEdit_textChanged(QString filter)
{
    if(filter.isEmpty())
    {
        int i = 0;
        QString sql = "select * from " + m_table + " order by id  asc";
        QSqlQuery q(sql);
        while(q.next())
        {
            m_model->setItem(i, 0, new QStandardItem(QString::number(q.value("id").toInt())));
            m_model->setItem(i, 1, new QStandardItem(q.value("name").toString()));

            ui->dbtableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            QLabel *label = new QLabel("");
            label->setFixedSize(100,80);
            label->setStyleSheet("border-image:url(" + m_image_path + q.value("image_path").toString() + ")");
            ui->dbtableview->setIndexWidget(m_model->index(m_model->rowCount() - 1, 2), label);

            ui->dbtableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

            m_model->setItem(i, 3, new QStandardItem(q.value("vip_rank").toString()));
            m_model->setItem(i, 4, new QStandardItem(q.value("vip_points").toString()));
            i++;
        }
        return;
    }
    //id name vip_rank vip_points
    QString modelFilter = QString("id like '%%1%' or name like '%%1%' or vip_rank like '%%1%' or vip_points like '%%1%'").arg(filter);
    FaceManage->setFilter(modelFilter);
    FaceManage->select();
    int rowCount = FaceManage->rowCount();
    if(rowCount==0)
    {  
        m_model->clear();
        QStringList columsTitles;
        columsTitles << "ID" << "姓名" << "照片" << "会员等级" << "会员积分";
        m_model->setHorizontalHeaderLabels(columsTitles);

    }

    for(int i=0; i<rowCount ;i++)
    {
        QModelIndex id = FaceManage->index(i,FaceManage->fieldIndex("id"));
        int Id = FaceManage->data(id).toInt();

        QModelIndex name = FaceManage->index(i,FaceManage->fieldIndex("name"));
        QString Name = FaceManage->data(name).toString();

        QModelIndex image_path = FaceManage->index(i,FaceManage->fieldIndex("image_path"));
        QString Image_path = FaceManage->data(image_path).toString();

        QModelIndex vip_rank = FaceManage->index(i,FaceManage->fieldIndex("vip_rank"));
        QString Vip_rank = FaceManage->data(vip_rank).toString();

        QModelIndex vip_points = FaceManage->index(i,FaceManage->fieldIndex("vip_points"));
        float Vip_points = FaceManage->data(vip_points).toFloat();

        //qDebug() << Id << Name << Vip_rank << Vip_points;

        m_model->setItem(i, 0, new QStandardItem(QString::number(Id)));
        m_model->setItem(i, 1, new QStandardItem(Name));

        ui->dbtableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QLabel *label = new QLabel("");
        label->setFixedSize(100,80);
        label->setStyleSheet("border-image:url(" + m_image_path + Image_path + ")");
        ui->dbtableview->setIndexWidget(m_model->index(m_model->rowCount() - 1, 2), label);

        ui->dbtableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        m_model->setItem(i, 3, new QStandardItem(Vip_rank));
        m_model->setItem(i, 4, new QStandardItem(QString::number(Vip_points)));
    }

}


