#include "client/controller.h"

static QString kLorem =
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque dictum luctus tortor eu lobortis. Vestibulum elementum nisl eros, quis sollicitudin sem tempor sed. Curabitur blandit malesuada dui ac maximus. Maecenas posuere, lorem nec viverra ultricies, arcu nisl sodales sem, vitae ullamcorper magna mauris a erat. Quisque sollicitudin sapien tellus, in vulputate erat maximus quis. Donec quis tincidunt dolor, non interdum ex. Etiam dictum luctus urna, sit amet cursus libero egestas a."
"In at sapien vel nisl pellentesque sollicitudin. Vivamus nisi tortor, vestibulum at augue vel, maximus suscipit nibh. Nam eget odio eu velit venenatis imperdiet. Nullam fermentum nibh vitae urna viverra, non malesuada lorem vestibulum. Maecenas auctor, enim sit amet rhoncus vulputate, felis ipsum aliquet nisl, sed egestas leo massa eu ipsum. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Integer quam nisl, tempus id turpis vitae, lacinia faucibus sapien. Etiam id faucibus erat. Suspendisse dignissim nisi vitae dolor hendrerit aliquam.\n"
"Curabitur congue efficitur sapien, a porta diam aliquet ac. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent aliquet vitae elit ac placerat. Duis in efficitur tortor. Integer sapien tellus, dictum tempus libero ac, scelerisque accumsan sapien. In elementum nibh non enim dapibus eleifend. Cras vestibulum dapibus volutpat. Quisque tristique, dui nec fringilla cursus, justo ex ultrices ipsum, ac iaculis felis velit sit amet risus. Fusce sodales justo mauris, sit amet imperdiet odio viverra quis. Proin eget quam vitae justo porta blandit in vitae est. Interdum et malesuada fames ac ante ipsum primis in faucibus. In hac habitasse platea dictumst. Aliquam erat volutpat.\n"
"Aliquam erat volutpat. Sed non laoreet nibh. Praesent semper neque ac hendrerit sagittis. Duis ligula eros, fringilla eu turpis ac, venenatis hendrerit libero. Sed condimentum pulvinar urna, ac commodo quam varius id. Vestibulum vel tellus euismod, fringilla magna ac, mollis arcu. Aliquam sodales, elit ac tempus sollicitudin, justo massa volutpat turpis, non molestie lorem ex non nisl. Nullam viverra orci id nulla auctor efficitur. Quisque ut sapien at ipsum blandit porttitor at egestas est. Sed et nisl a risus pellentesque sagittis. Ut fringilla efficitur ornare. Fusce scelerisque commodo elit tempor cursus. Mauris quis faucibus odio. Vivamus malesuada ipsum velit, in semper purus pharetra sit amet. Interdum et malesuada fames ac ante ipsum primis in faucibus. Curabitur mollis viverra tortor nec tempus.\n"
"Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Suspendisse fermentum est lacus, sed volutpat velit luctus luctus. Sed id neque vestibulum libero eleifend gravida. Sed laoreet malesuada elit vitae condimentum. Sed vel elit a sapien fermentum facilisis laoreet quis ipsum. Nam rutrum nec magna a malesuada. Proin sodales sapien in orci tempus, id tristique lorem aliquet. Aliquam ut lacus rhoncus, auctor purus non, ultricies lacus. Integer et urna nec odio dapibus feugiat id eget lorem. Aenean non diam hendrerit velit varius egestas. Praesent urna sem, dictum vitae efficitur laoreet, efficitur non ex. Sed malesuada aliquam maximus. Sed quis pellentesque velit. Sed ac leo tempus, venenatis dui at, maximus magna.";


Controller::Controller(QObject *parent) : QObject(parent)
{

}

ServerConnectionResult* Controller::connectToServer(QString address, QString port)
{

    return new ServerConnectionResult(parent(), address, port, true, "Ok");
}

AuthenticationResult* Controller::authenticate(QString login, QString password)
{

    return new AuthenticationResult(parent(), login, true, "Ok");
}

FileInfo* Controller::loadFile(QString filename)
{

    return new FileInfo(parent(), filename, kLorem);
}

void Controller::reset()
{
    // TODO: implement
}

