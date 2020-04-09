#ifndef LOGIN_H
#define LOGIN_H

#include <qaesencryption.h>

#include <QMainWindow>
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileSystemWatcher>
#include <QDirIterator>
#include <QDir>
#include <QFile>

#include <windows.h>
#include <winioctl.h>
#include <initguid.h>
#include <string.h>
#include <fstream>

#include <virtdisk.h>
#pragma comment(lib, "virtdisk.lib")
#include <rpc.h>
#pragma comment(lib,"Rpcrt4.lib")
#include <sddl.h>
#pragma comment(lib,"Advapi32.lib")
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <ShObjIdl_core.h>
#pragma comment(lib, "Shell32.lib")

using namespace std;

#define DISKNAME "My Vault.vhd"
#define VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT {0xec984aec, 0xa0f9, 0x47e9, {0x90, 0x1f, 0x71, 0x41, 0x5a, 0x66, 0x34, 0x5b}}

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_goto_reg_clicked();
    void on_goto_log_clicked();
    void on_login_clicked();


private:
    Ui::Login *ui;


    //Virtual Hard Disk
    DWORD SampleCreateVirtualDisk(_In_ LPCWSTR VirtualDiskPath, _In_opt_ LPCWSTR ParentPath, _In_ CREATE_VIRTUAL_DISK_FLAG Flags, _In_ ULONGLONG FileSize,
                                  _In_ DWORD BlockSize, _In_ DWORD LogicalSectorSize, _In_ DWORD PhysicalSectorSize);
    unsigned long create_v_disk(LPCWSTR virtualDiskPath, ULONGLONG fileSize_mb);
    void generate_diskpart_script(string script, string path);
    void run_diskpart_script(string path);
    void format_v_disk(string path_str, string diskName);
    void attach_v_disk(string path_str, string diskName);
    void detach_v_disk(string path_str, string diskName);
    void init_v_disk(string path, string diskName);

    void on_change(const QString &path);

    //Encrypt/Decrypt file
    QByteArray data_read(QString path);
    void data_write(QByteArray data, QString path);


};
#endif // LOGIN_H
