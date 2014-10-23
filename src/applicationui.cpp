/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <QStringList>

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
        QObject()
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("CascadesProject_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::setText(QString newValue)
{
    qDebug() << newValue;

    QString celsius = QString::number(floor(newValue.toDouble()));
    QString farenheit = QString::number(floor((newValue.toDouble() * 1.8) + 32));
    QByteArray degreeSign = "°";

    QStringList listOfMessages;
    int indexOfTheMessageToShow;

    if (newValue <= "-50") {
        listOfMessages.clear();
        listOfMessages << "Hell is freezing the fuck over (%1%2C/%3%2F)" << "Even ice is saying fuck that, I'm not going ouside today (%1%2C/%3%2F)" << "Is that for real? (%1%2C/%3%2F)";

        indexOfTheMessageToShow = qrand() % listOfMessages.size();
    }
    else if ((newValue <= "-20") && (newValue > "-50")) {
        listOfMessages.clear();
        listOfMessages << "Where do you live fucker? Fucking Artantic? (%1%2C/%3%2F)" << "Are you serious (%1%2C/%3%2F)" << "Don't take a piss outside at %1%2C/%3%2F";

        indexOfTheMessageToShow = qrand() % listOfMessages.size();
    }
    else if ((newValue <= "0") && (newValue > "-20")) {
        listOfMessages.clear();
        listOfMessages << "Fuck that's cold (%1%2C/%3%2F)" << "Even water is freezing at this point (%1%2C/%3%2F)" << "You're in a Igloo or what? (%1%2C/%3%2F)";

        indexOfTheMessageToShow = qrand() % listOfMessages.size();
    }
    else if ((newValue <= "20") && (newValue > "0")) {
        listOfMessages.clear();
        listOfMessages << "It's fucking chilly (%1%2C/%3%2F)" << "I hope you don't think it's summer (%1%2C/%3%2F)" << "That's not even hot (%1%2C/%3%2F)";

        indexOfTheMessageToShow = qrand() % listOfMessages.size();
    }
    else if ((newValue <= "50") && (newValue > "20")) {
        listOfMessages.clear();
        listOfMessages << "That's Jennifer Lopez hot (%1%2C/%3%2F)" << "It's fucking melting outside (%1%2C/%3%2F)" << "Get a freaking A/C (%1%2C/%3%2F)";

        indexOfTheMessageToShow = qrand() % listOfMessages.size();
    }
    else if (newValue > "50") {
        listOfMessages.clear();
        listOfMessages << "It's hell over here (%1%2C/%3%2F)" << "Smoking (%1%2C/%3%2F)" << "Burning like fucking toast in a toaster (%1%2C/%3%2F)";

        indexOfTheMessageToShow = qrand() % listOfMessages.size();
    }

    m_text = listOfMessages[indexOfTheMessageToShow].arg(celsius, QString::fromUtf8(degreeSign), farenheit);
    emit textChanged();
}
