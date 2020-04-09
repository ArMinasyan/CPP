#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent): QMainWindow(parent), ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->reg_group->hide();
    ui->reg_group->setGeometry(20,70,291,200);

///
   // QStringList list("Z:\test1,Z:\test2");

}

Login::~Login()
{
    delete ui;
}

void Login::on_goto_reg_clicked()
{
    ui->reg_group->show();
    ui->login_group->hide();
}

void Login::on_goto_log_clicked()
{
    ui->reg_group->setGeometry(20,70,291,200);
    ui->reg_group->hide();
    ui->login_group->show();


}

void Login::on_change(const QString &path){
   // QString paths="Z:\test1,Z:\test2";
   //qDebug()<<path;

}

void Login::on_login_clicked()
{
    QMessageBox msg;
    QNetworkAccessManager *am=new QNetworkAccessManager;
    QJsonObject *obj=new QJsonObject;
       QNetworkRequest *req=new QNetworkRequest;
       QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
       sslConfiguration.setProtocol(QSsl::TlsV1_2);

    if(ui->username->text().length()==0 || ui->password->text().length()==0)
        msg.warning(0,"Empty field(s)","Please, input all fields");
    else{
        obj->insert("username",ui->username->text());
        obj->insert("password",ui->password->text());
      //  req->setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
       // req->setUrl(QUrl("https://cpp-app.herokuapp.com/login"));
      //  QNetworkReply *rep=am->post(*req,QJsonDocument(*obj).toJson());

      //  connect(rep,&QNetworkReply::finished,[=](){
            QMessageBox result_msg;
      //      QString result=rep->readAll();

       //     QJsonObject json(QJsonDocument::fromJson(result.toUtf8()).object());
            if(ui->username->text()!="admin" && ui->password->text()!="admin")//json.value("result")!=true
                result_msg.critical(0,"Credinals","Incorrect username and/or password, please try again");
        else
            {
              //  qDebug()<<"true";
                QMessageBox error_msg;
                char pathBuf[512];
                LPSTR filePath = pathBuf;
                GetModuleFileNameA(NULL, filePath, 512);
                string path_str(pathBuf);
                auto pos = path_str.find_last_of("\\");
                path_str.erase(pos + 1, path_str.length() - pos + 1);

                LPCWSTR virtualDiskPath = L"./My Vault.vhd";                            // need to lay where exe is
                ULONGLONG fileSize = 100;


                auto code = create_v_disk(virtualDiskPath, fileSize);
                  if (code != ERROR_SUCCESS) {
                      error_msg.critical(0,"Error","Try again");
                }
                    init_v_disk(path_str, DISKNAME);

                    QDir obj;
                    obj.mkdir("Z:/Cloud");
            }
 //       });
    }
}



