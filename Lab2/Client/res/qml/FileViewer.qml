import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.12

Rectangle {
    anchors.fill: parent
    color: "azure"

    signal back()

    function setFile(filename, text) {
        labelFilename.text = filename
        textAreaFileContents.text = text
    }

    Label {
        id: labelFilename
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 5
            bottomMargin: 5
        }
        text: qsTr("<FILENAME>")
    }

    ScrollView {
        id: scrollViewFileContents
        width: parent.width
        anchors {
            top: labelFilename.bottom;
            bottom: buttonBack.top
            margins: 5
        }
        Text{
            id: textAreaFileContents
            anchors.fill: parent
            text:
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus laoreet ullamcorper lectus et faucibus. Mauris ac pharetra enim, vitae vestibulum tortor.
In et convallis erat, ut suscipit risus. Aenean sem neque, dictum sit amet pretium suscipit, iaculis sit amet quam. Aenean tempor justo a odio fringilla fringilla.
Donec hendrerit tellus id felis interdum, ut vehicula nibh varius. Fusce quis arcu in arcu pretium bibendum. Aenean sed vehicula nibh.
Sed id enim id orci viverra molestie vulputate in tortor. Sed tincidunt malesuada lacus, sed vestibulum nisi tincidunt et. Praesent vitae dolor augue.
Donec dui urna, tempus id diam sit amet, pharetra eleifend felis. Ut egestas urna non cursus finibus. Phasellus tellus lectus, tempus ut est eu, vestibulum scelerisque massa.
Duis tellus tellus, consequat ut malesuada vitae, aliquam ut nunc. Nullam egestas consectetur purus, tristique iaculis nisl mollis vel.
Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Suspendisse vel sagittis dolor, vitae facilisis diam. Nunc auctor risus ac volutpat sagittis.
Interdum et malesuada fames ac ante ipsum primis in faucibus. Etiam neque nisl, rhoncus sit amet luctus sodales, iaculis a lorem.
Etiam eget imperdiet nulla. Phasellus sapien ante, sollicitudin non nibh non, mollis consectetur felis. Praesent eu lectus blandit, imperdiet nisi id, elementum nisl.
Quisque gravida molestie augue, quis aliquet enim vehicula vitae. Aliquam semper justo sed magna aliquam, et feugiat lectus hendrerit.
Sed quis nunc sed ligula scelerisque dapibus sed id diam. Nam in augue sem. Nunc turpis sem, feugiat lacinia auctor sit amet, egestas a massa. Integer interdum at nibh quis facilisis.
Sed at purus pulvinar, suscipit purus elementum, venenatis magna. Phasellus condimentum vulputate nisl, eu dignissim augue ornare non.
Suspendisse ultrices lectus eget neque sollicitudin, lacinia porta neque cursus. Pellentesque non euismod dui. Nam faucibus fermentum eros eget gravida. Sed placerat ligula sed vehicula lacinia.
Cras fringilla augue a erat imperdiet, a tempus est interdum.
Donec consequat arcu odio, ac dapibus sapien pellentesque gravida. In sed porttitor dui. Aliquam erat volutpat. Nam fermentum velit massa, feugiat porttitor orci venenatis eu.
Maecenas risus eros, dapibus vitae augue ac, tincidunt dignissim nisi. Suspendisse venenatis laoreet finibus. Proin orci lectus, gravida a nulla in, pharetra aliquet nunc.
Integer non vestibulum ligula. Quisque sit amet posuere libero. Praesent ut luctus odio. Donec lobortis tellus id ipsum posuere tincidunt.
Fusce diam ante, porttitor at tellus ac, venenatis porta lorem. Nunc viverra viverra pharetra. Mauris euismod dolor efficitur justo vestibulum vestibulum. Vestibulum tempus sed orci eu pretium.
Nullam lorem nisl, ullamcorper at auctor eu, luctus ut massa. Etiam ultrices, lectus ac gravida facilisis, quam quam posuere urna, eget rhoncus enim turpis nec lorem.
Ut at mauris mauris. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Vestibulum vitae nulla sem. Nam varius condimentum consectetur.
Ut feugiat commodo erat nec laoreet. Quisque maximus eros tortor, vel porttitor ante iaculis et. Nullam luctus dictum risus in laoreet."
        }
    }
    Button {
        id: buttonBack
        width: parent.width
        anchors {
            bottom: parent.bottom
        }
        text: qsTr("Back")
        onClicked: back()
    }
} // Rectangle
