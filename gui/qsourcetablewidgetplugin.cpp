#include "qsourcetablewidget.h"
#include "qsourcetablewidget.h"

#include <QtPlugin>

QSourceTableWidgetPlugin::QSourceTableWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QSourceTableWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QSourceTableWidgetPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QSourceTableWidgetPlugin::createWidget(QWidget *parent)
{
    return new WorldTimeClock(parent);
}

QString QSourceTableWidgetPlugin::name() const
{
    return "QSourceTableWidget";
}

QString QSourceTableWidgetPlugin::group() const
{
    return "Simplex";
}

QIcon QSourceTableWidgetPlugin::icon() const
{
    return QIcon();
}

QString QSourceTableWidgetPlugin::toolTip() const
{
    return "";
}

QString QSourceTableWidgetPlugin::whatsThis() const
{
    return "";
}

bool QSourceTableWidgetPlugin::isContainer() const
{
    return false;
}

QString QSourceTableWidgetPlugin::domXml() const
{
    return "";
}

QString QSourceTableWidgetPlugin::includeFile() const
{
    return "qsourcetablewidget.h";
}

Q_EXPORT_PLUGIN2(QSourceTableWidgetPlugin, QSourceTableWidgetPlugin)

