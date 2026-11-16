#include "tcpfileserver.h"
#include <QDialogButtonBox>
#include <QHostAddress>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
namespace {
constexpr quint16 ServerPort = 16689;
}
TcpFileServer::TcpFileServer(QWidget *parent)
    : QDialog(parent),
      statusLabel(new QLabel(tr("伺服器端就緒"), this)),
      startButton(new QPushButton(tr("接收"), this))
{
    auto *quitButton = new QPushButton(tr("退出"), this);
    auto *buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(statusLabel);
    layout->addWidget(buttonBox);
    setWindowTitle(tr("接收檔案"));
    setMinimumWidth(300);
    connect(startButton, &QPushButton::clicked, this, &TcpFileServer::start);
    connect(quitButton, &QPushButton::clicked, this, &QDialog::close);
}
void TcpFileServer::start()
{
    startButton->setEnabled(false);
    while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::AnyIPv4, ServerPort)) {
        const QString listenErrorText =
            tr("無法監聽連接埠 %1\n%2").arg(ServerPort).arg(tcpServer.errorString());
        QMessageBox listenErrorDialog(QMessageBox::Critical, tr("無法啟動伺服器"),
                                      listenErrorText, QMessageBox::Retry | QMessageBox::Cancel,
                                      this);
        listenErrorDialog.button(QMessageBox::Retry)->setText(tr("重試"));
        listenErrorDialog.button(QMessageBox::Cancel)->setText(tr("取消"));
        if (listenErrorDialog.exec() != QMessageBox::Retry) {
            startButton->setEnabled(true);
            return;
        }
    }
    statusLabel->setText(tr("監聽中...（TCP 連接埠 %1）").arg(tcpServer.serverPort()));
}
