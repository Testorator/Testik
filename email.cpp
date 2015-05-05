#include "email.h"
#include <QDebug>
#include <QRegExpValidator>

bool address_correct(QString addr)
{
    QRegExp rx_std("^([a-z0-9_-]+\.)*[a-z0-9_-]+@[a-z0-9_-]+(\.[a-z0-9_-]+)*\.[a-z]{2,6}$");
    QRegExpValidator v(rx_std,0);

    int pos = 0;
    bool result = false;
    if(v.validate(addr,pos) == 2){
        result = true;
    }

    return result;
}

email::email(QObject *parent) : QObject(parent)
{

}

email::~email()
{

}
//
bool email::sendMessage(QString *subject, QStringList *addreses, QString *msg)
{
    bool result = false;
    socket = new QTcpSocket( this );

    delete socket;
    return result;
}
//
//Smtp::Smtp( const QString &from, const QString &to, const QString &subject, const QString &body )
//{
//

//    connect( socket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
//    connect( socket, SIGNAL( connected() ), this, SLOT( connected() ) );
//    connect( socket, SIGNAL(error(SocketError)), this,
//		SLOT(errorReceived(SocketError)));
//    connect( socket, SIGNAL(stateChanged( SocketState)), this,
//		SLOT(stateChanged(SocketState)));
//    connect(socket, SIGNAL(disconnectedFromHost()), this,
//		SLOT(disconnected()));;

//	message = "To: " + to + "\n";
//	message.append("From: " + from + "\n");
//	message.append("Subject: " + subject + "\n");
// 	message.append(body);
//	message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
//	message.replace( QString::fromLatin1( "\r\n.\r\n" ),
//	QString::fromLatin1( "\r\n..\r\n" ) );
//    this->from = from;
//    rcpt = to;
//    state = Init;
//    socket->connectToHost( "smtp.yourserver.com", 25);
//    if(socket->waitForConnected ( 30000 )) 	{qDebug("connected"); 	}

//    t = new QTextStream( socket );
//}
//Smtp::~Smtp()
//{
//    delete t;
//
//}
void email::stateChanged(QTcpSocket::SocketState socketState)
{

    qDebug() <<"stateChanged " << socketState;
}
//
void email::errorReceived(QTcpSocket::SocketError socketError)
{
    qDebug() << "error " <<socketError;
}
//
void email::disconnected()
{
    qDebug() <<"disconneted";
    qDebug() << "error "  << socket->errorString();
}
//
void email::connected()
{
    output->append("connected");
    qDebug() << "Connected ";
}

void email::readyRead()
{

//     qDebug() <<"readyRead";
//    // SMTP is line-oriented

//    QString responseLine;
//    do
//    {
//        responseLine = socket->readLine();
//        response += responseLine;
//    }
//    while ( socket->canReadLine() && responseLine[3] != ' ' );

//	responseLine.truncate( 3 );


//    if ( state == Init && responseLine[0] == '2' )
//    {
//        // banner was okay, let's go on

//		*t << "HELO there\r\n";
//		t->flush();

//        state = Mail;
//    }
//    else if ( state == Mail && responseLine[0] == '2' )
//    {
//        // HELO response was okay (well, it has to be)

//        *t << "MAIL FROM: " << from << "\r\n";
//		t->flush();
//        state = Rcpt;
//    }
//    else if ( state == Rcpt && responseLine[0] == '2' )
//    {

//        *t << "RCPT TO: " << rcpt << "\r\n"; //r
//		t->flush();
//        state = Data;
//    }
//    else if ( state == Data && responseLine[0] == '2' )
//    {

//        *t << "DATA\r\n";
//		t->flush();
//        state = Body;
//    }
//    else if ( state == Body && responseLine[0] == '3' )
//    {

//        *t << message << "\r\n.\r\n";
//		t->flush();
//        state = Quit;
//    }
//    else if ( state == Quit && responseLine[0] == '2' )
//    {

//        *t << "QUIT\r\n";
//		t->flush();
//        // here, we just close.
//        state = Close;
//        emit status( tr( "Message sent" ) );
//    }
//    else if ( state == Close )
//    {
//        deleteLater();
//        return;
//    }
//    else
//    {
//        // something broke.
//        QMessageBox::warning( 0, tr( "Qt Mail Example" ), tr( "Unexpected reply from SMTP server:\n\n" ) + response );
//        state = Close;
//    }
//    response = "";
}
