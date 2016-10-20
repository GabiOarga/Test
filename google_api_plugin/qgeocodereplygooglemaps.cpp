#include "qgeocodereplygooglemaps.h"

#include <QtPositioning/QGeoCoordinate>
#include <QtPositioning/QGeoAddress>
#include <QtPositioning/QGeoLocation>
#include <QtPositioning/QGeoRectangle>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

/*Request
location: -33.8670,151.1957
radius: 500
types: food
name: cruise
key: API_KEY*/

//https://maps.googleapis.com/maps/api/place/nearbysearch/json?location=-33.8670,151.1957&radius=500&types=food&name=cruise&key=YOUR_API_KEY

/*Response Example
{
   "debug_log" : {
      "line" : []
   },
   "html_attributions" : [],
   "logging_info" : {
      "experiment_id" : [],
      "query_geographic_location" : "AU"
   },
   "results" : [
      {
         "geometry" : {
            "location" : {
               "lat" : -33.86879,
               "lng" : 151.194217
            }
         },
         "icon" : "http://maps.gstatic.com/mapfiles/place_api/icons/restaurant-71.png",
         "id" : "21a0b251c9b8392186142c798263e289fe45b4aa",
         "name" : "Rhythmboat Cruises",
         "opening_hours" : {
            "open_now" : false,
            "weekday_text" : []
         },
         "photos" : [
            {
               "height" : 426,
               "html_attributions" : [
                  "\u003ca href=\"https://www.google.com/maps/views/profile/104066891898402903288\"\u003eRhythmboat Cruises\u003c/a\u003e"
               ],
               "photo_reference" : "CmRdAAAA-YL_I ...photo_reference truncated in this example",
               "width" : 640
            }
         ],
         "place_id" : "ChIJyWEHuEmuEmsRm9hTkapTCrk",
         "reference" : "CnRmAAAAvQl ...reference truncated in this example",
         "scope" : "GOOGLE",
         "types" : [ "restaurant", "food", "point_of_interest", "establishment" ],
         "vicinity" : "Pyrmont Bay Wharf (Near Australia Maritime Museum), Pyrmont, NSW 2009"
      },
      {
         "geometry" : {
            "location" : {
               "lat" : -33.867591,
               "lng" : 151.201196
            }
         },
         "icon" : "http://maps.gstatic.com/mapfiles/place_api/icons/restaurant-71.png",
         "id" : "a97f9fb468bcd26b68a23072a55af82d4b325e0d",
         "name" : "Australian Cruise Group",
         "opening_hours" : {
            "open_now" : false,
            "weekday_text" : []
         },
         "photos" : [
            {
               "height" : 1331,
               "html_attributions" : [
                  "\u003ca href=\"https://www.google.com/maps/views/profile/110751364053842618118\"\u003eAustralian Cruise Group\u003c/a\u003e"
               ],
               "photo_reference" : "CmRdAAAAtEo ...photo_reference truncated in this example",
               "width" : 2000
            }
         ],
         "place_id" : "ChIJrTLr-GyuEmsRBfy61i59si0",
         "reference" : "CnRqAAAAFbuk ...reference truncated in this example",
         "scope" : "GOOGLE",
         "types" : [
            "restaurant",
            "travel_agency",
            "food",
            "point_of_interest",
            "establishment"
         ],
         "vicinity" : "32 The Promenade, King Street Wharf 5, Sydney"
      },
      {
         "geometry" : {
            "location" : {
               "lat" : -33.870943,
               "lng" : 151.190311
            }
         },
         "icon" : "http://maps.gstatic.com/mapfiles/place_api/icons/bar-71.png",
         "id" : "e644f7f34cf875b9919c6548f1b721947362850a",
         "name" : "Lunch Cruise with Jazz on Sydney Harbour",
         "opening_hours" : {
            "open_now" : false,
            "weekday_text" : []
         },
         "photos" : [
            {
               "height" : 292,
               "html_attributions" : [
                  "\u003ca href=\"https://www.google.com/maps/views/profile/105423912060796272053\"\u003eFrom a Google User\u003c/a\u003e"
               ],
               "photo_reference" : "CmRdAAAAR4bq ...photo_reference truncated in this example",
               "width" : 438
            }
         ],
         "place_id" : "ChIJLfySpTOuEmsRPCRKrzl8ZEY",
         "reference" : "CoQBewAAAFd2 ...reference truncated in this example",
         "scope" : "GOOGLE",
         "types" : [ "bar", "restaurant", "food", "point_of_interest", "establishment" ],
         "vicinity" : "37 Bank St, Pyrmont"
      }
   ],
   "status" : "OK"
}*/

static QGeoCoordinate constructCoordiante(const QJsonObject &jsonCoord) {
    QGeoCoordinate coord(0,0);
    coord.setLatitude(jsonCoord.value(QStringLiteral("lat")).toDouble());
    coord.setLongitude(jsonCoord.value(QStringLiteral("lng")).toDouble());
    return coord;
}

static bool checkAddressType(const QJsonObject &jsonAddress, const QString &type) {
    QJsonArray a = jsonAddress.value("types").toArray();
    for (int i = 0; i < a.size(); i++) {
        if (a.at(i).toString() == type)
            return true;
    }
    return false;
}

QGeoCodeReplyGooglemaps::QGeoCodeReplyGooglemaps(QNetworkReply *reply, QObject *parent)
    :   QGeoCodeReply(parent), m_reply(reply)
{
    connect(m_reply, SIGNAL(finished()), this, SLOT(networkReplyFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(networkReplyError(QNetworkReply::NetworkError)));

    setLimit(1);
    setOffset(0);
}

QGeoCodeReplyGooglemaps::~QGeoCodeReplyGooglemaps()
{
    if (m_reply)
        m_reply->deleteLater();
}

void QGeoCodeReplyGooglemaps::abort()
{
    if (!m_reply)
        return;

    m_reply->abort();

    m_reply->deleteLater();
    m_reply = 0;
}


void QGeoCodeReplyGooglemaps::networkReplyFinished()
{
    if (!m_reply)
        return;

    if (m_reply->error() != QNetworkReply::NoError)
        return;

    QString status;

    QList<QGeoLocation> locations;
    //        setError(QGeoCodeReply::ParseError, QStringLiteral("Error parsing OpenRouteService xml response:") + xml.errorString() + " at line: " + xml.lineNumber());
    QJsonDocument document = QJsonDocument::fromJson(m_reply->read(m_reply->bytesAvailable()));
    if (document.isObject()) {
        QJsonObject object = document.object();

        status = object.value(QStringLiteral("status")).toString();
        if (status == "OK") {
            QJsonArray jsonlocations = object.value(QStringLiteral("results")).toArray();
            qDebug() << "locations:" << jsonlocations.size();
            for(int i = 0; i < jsonlocations.size(); i++) {
                QGeoLocation location;
                QGeoAddress address;

                QJsonObject o = jsonlocations.at(i).toObject();
                address.setText(o.value("formatted_address").toString());
                QJsonObject ogeometry = o.value("geometry").toObject();

                location.setCoordinate(constructCoordiante(ogeometry.value("location").toObject()));

                QJsonObject jaddressRanges = ogeometry.value("viewport").toObject();
                if (!jaddressRanges.isEmpty()) {
                    QGeoRectangle r;
                    r.setTopRight(constructCoordiante(jaddressRanges.value("northeast").toObject()));
                    r.setBottomLeft(constructCoordiante(jaddressRanges.value("southwest").toObject()));
                    location.setBoundingBox(r);
                }

                QJsonArray jaddress = o.value("address_components").toArray();

                QString street_name;
                QString street_num;
                for(int j = 0; j < jaddress.size(); j++) {
                    QJsonObject addobj = jaddress.at(j).toObject();
                    if (checkAddressType(addobj, "street_number"))
                        street_num = addobj.value("long_name").toString();
                    if (checkAddressType(addobj, "route"))
                        street_name = addobj.value("long_name").toString();
                    if (checkAddressType(addobj, "country"))
                        address.setCountryCode(addobj.value("short_name").toString());
                    if (checkAddressType(addobj, "administrative_area_level_3"))
                        address.setCity(addobj.value("long_name").toString());
                    if (checkAddressType(addobj, "administrative_area_level_2"))
                        address.setCounty(addobj.value("long_name").toString());
                    if (checkAddressType(addobj, "administrative_area_level_1"))
                        address.setState(addobj.value("long_name").toString());
                    if (checkAddressType(addobj, "sublocality"))
                        address.setDistrict(addobj.value("long_name").toString());
                    if (checkAddressType(addobj, "postal_code"))
                        address.setPostalCode(addobj.value("long_name").toString());
                }
                address.setStreet(street_name + (street_num.size() > 0 ? ( ", " + street_num) : ""));

                location.setAddress(address);

                locations.append(location);
            }
        }
    } else {
        if (status == "ZERO_RESULTS")
            setError(CombinationError, "Geocode was successful but returned no results. This may occur if the geocoder was passed a non-existent address");
        else if (status == "OVER_QUERY_LIMIT")
            setError(CommunicationError, "Request quota is over");
        else if (status == "REQUEST_DENIED")
            setError(CommunicationError, "Request denied");
        else if (status == "INVALID_REQUEST")
            setError(UnsupportedOptionError, "Address, components or latlng is missing");
        else if (status == "UNKNOWN_ERROR")
            setError(UnknownError, "Request could not be processed due to a server error. Try again later");
    }

    setLocations(locations);
    setFinished(true);

    m_reply->deleteLater();
    m_reply = 0;
}

void QGeoCodeReplyGooglemaps::networkReplyError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)

    if (!m_reply)
        return;

    setError(QGeoCodeReply::CommunicationError, m_reply->errorString());

    m_reply->deleteLater();
    m_reply = 0;
}
