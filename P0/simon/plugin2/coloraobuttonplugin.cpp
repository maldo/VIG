#include "coloraobutton.h"
#include "coloraobuttonplugin.h"

#include <QtCore/QtPlugin>

ColoraoButtonPlugin::ColoraoButtonPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void ColoraoButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ColoraoButtonPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ColoraoButtonPlugin::createWidget(QWidget *parent)
{
    return new ColoraoButton(parent);
}

QString ColoraoButtonPlugin::name() const
{
    return QLatin1String("coloraoButton");
}

QString ColoraoButtonPlugin::group() const
{
    return QLatin1String("Mis Cosas");
}

QIcon ColoraoButtonPlugin::icon() const
{
    return QIcon();
}

QString ColoraoButtonPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ColoraoButtonPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ColoraoButtonPlugin::isContainer() const
{
    return false;
}

QString ColoraoButtonPlugin::domXml() const
{
    return QLatin1String("<widget class=\"ColoraoButton\" name=\"coloraoButton\">\n</widget>\n");
}

QString ColoraoButtonPlugin::includeFile() const
{
    return QLatin1String("coloraobutton.h");
}

Q_EXPORT_PLUGIN2(Coloraobuttonplugin, ColoraoButtonPlugin)
