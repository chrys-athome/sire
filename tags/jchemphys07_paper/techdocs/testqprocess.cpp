
#include <QProcess>
#include <QString>
#include <QStringList>

#include <QDebug>

int main(int argc, char **argv)
{
    QString cmd = "/home/chzcjw/software/molpro/devel/molpro";
    
    QProcess proc;
    
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.setWorkingDirectory("/tmp");
    
    QStringList env = QProcess::systemEnvironment();
    env.append("TMPDIR=.");
    
    proc.setEnvironment(env);
    
    proc.start(cmd, QIODevice::ReadWrite);
    
    proc.waitForStarted();
    
    proc.write( "user\n" );
    
    proc.closeWriteChannel();
    
    while (proc.waitForReadyRead(-1))
    {
        while (proc.canReadLine())
        {
            qDebug() << proc.readLine();
        }
    }


    return 0;
}
