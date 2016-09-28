#ifndef CYCOREPLUGIN_H
#define CYCOREPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>

class CYCorePlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "cycoreplugin.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    CYCorePlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool initialized;
};

#endif // CYCOREPLUGIN_H
