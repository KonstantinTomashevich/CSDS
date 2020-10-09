#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "server_connection_result.h"
#include "authentication_result.h"
#include "file_info.h"

class Controller : public QObject
{
    Q_OBJECT

    /**
     * @brief The Screen enum
     * Used to reference ui state.
     */
    enum class Screen : char {
        SERVER_CHOOSE = 0,
        AUTHENTICATE = 1,
        LOGIN = 2,
        FILE_CHOOSE = 3,
        FILE_VIEW = 4
    };

public:
    explicit Controller(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE ServerConnectionResult* connectToServer(QString address, QString port);
    Q_INVOKABLE AuthenticationResult* authenticate(QString login, QString password);
    Q_INVOKABLE FileInfo* loadFile(QString filename);
    Q_INVOKABLE void reset();

private:
    /// Current ui state
    Screen _activeScreen;
};

#endif // CONTROLLER_H
