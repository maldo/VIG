#include "bluelabel.h"
#include "bluelabelplugin.h"

#include <QtCore/QtPlugin>

BlueLabelPlugin::BlueLabelPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void BlueLabelPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool BlueLabelPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *BlueLabelPlugin::createWidget(QWidget *parent)
{
    return new BlueLabel(parent);
}

QString BlueLabelPlugin::name() const
{
    return QLatin1String("blueLabel");
}

QString BlueLabelPlugin::group() const
{
    return QLatin1String("Mis Cosas");
}

QIcon BlueLabelPlugin::icon() const
{
    return QIcon();
}

QString BlueLabelPlugin::toolTip() const
{
    return QLatin1String("");
}

QString BlueLabelPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool BlueLabelPlugin::isContainer() const
{
    return false;
}

QString BlueLabelPlugin::domXml() const
{
    return QLatin1String("<widget class=\"BlueLabel\" name=\"blueLabel\">\n</widget>\n");
}

QString BlueLabelPlugin::includeFile() const
{
    return QLatin1String("bluelabel.h");
}

Q_EXPORT_PLUGIN2(BlueLabelPlugin, BlueLabelPlugin)
