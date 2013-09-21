#include "glwidget.h"
#include "glwidgetplugin.h"

#include <QtCore/QtPlugin>

GLWidgetPlugin::GLWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void GLWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool GLWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *GLWidgetPlugin::createWidget(QWidget *parent)
{
    return new GLWidget(parent);
}

QString GLWidgetPlugin::name() const
{
    return QLatin1String("GLWidget");
}

QString GLWidgetPlugin::group() const
{
    return QLatin1String("Els Meus Widgets");
}

QIcon GLWidgetPlugin::icon() const
{
    return QIcon();
}

QString GLWidgetPlugin::toolTip() const
{
    return QLatin1String("");
}

QString GLWidgetPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool GLWidgetPlugin::isContainer() const
{
    return false;
}

QString GLWidgetPlugin::domXml() const
{
    return QLatin1String("<widget class=\"GLWidget\" name=\"gLWidget\">\n</widget>\n");
}

QString GLWidgetPlugin::includeFile() const
{
    return QLatin1String("glwidget.h");
}

Q_EXPORT_PLUGIN2(glwidgetplugin, GLWidgetPlugin)
