#include "camyupropertyedit.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHash>
#include <QDebug>

#include "QtTreePropertyBrowser.h"
#include "designerpropertymanager.h"
namespace CAMYUPROPEDIT{
class CamyuPropertyEditPrivate : public QObject
{
    Q_OBJECT
public:
    QLineEdit *m_filterWidget = nullptr;
    QtTreePropertyBrowser * m_treeBrowser = nullptr;
    qdesigner_internal::DesignerPropertyManager *m_propertyManager = nullptr;
    qdesigner_internal::DesignerEditorFactory *m_treeFactory = nullptr;
    QtVariantProperty * m_Property = nullptr;

    QString m_filterPattern;
    QHash<QString, QtVariantProperty*> m_nameToProperty;
public:
    void setFilter(const QString &pattern)
    {
        m_filterPattern = pattern;
        applyFilter();
    }
    void slotResetProperty(QtProperty *property)
    {
        qDebug("slotResetProperty\n");
    }
    void slotValueChanged(QtProperty *property, const QVariant &value, bool enableSubPropertyHandling)
    {
        QString value_str = value.toString();
        //m_nameToProperty.value(property, 0);
        QtVariantProperty *varProp = m_propertyManager->variantProperty(property);
        QString nameKey = m_nameToProperty.key(varProp);
    }
private:
    void applyFilter()
    {
        const QList<QtBrowserItem *> items = m_treeBrowser->topLevelItems();
        if (0) {
            applyPropertiesFilter(items);
        }
        else {
            QListIterator<QtBrowserItem *> itTopLevel(items);
            //applyPropertiesFilter(items);
            while (itTopLevel.hasNext()) {
                QtBrowserItem *item = itTopLevel.next();
                setItemVisible(item, applyPropertiesFilter(item->children()));
            }
        }
    }
    int applyPropertiesFilter(const QList<QtBrowserItem *> &items)
    {
        int showCount = 0;
        const bool matchAll = m_filterPattern.isEmpty();
        QListIterator<QtBrowserItem *> itProperty(items);
        while (itProperty.hasNext()) {
            QtBrowserItem *propertyItem = itProperty.next();
            QtProperty *property = propertyItem->property();
            const QString propertyName = property->propertyName();
            const bool showProperty = matchAll || propertyName.contains(m_filterPattern, Qt::CaseInsensitive);
            setItemVisible(propertyItem, showProperty);
            if (showProperty)
                showCount++;
        }
        return showCount;
    }
    void setItemVisible(QtBrowserItem *item, bool visible)
    {
        if (m_treeBrowser == m_treeBrowser) {
            m_treeBrowser->setItemVisible(item, visible);
        }
        else {
            qWarning("** WARNING %s is not implemented for this browser.", Q_FUNC_INFO);
        }
    }
};

CamyuPropertyEdit::CamyuPropertyEdit():
    d(new CamyuPropertyEditPrivate)
{
    // 配置ui
    d->m_filterWidget = new QLineEdit(this);    //搜索框
    d->m_treeBrowser = new QtTreePropertyBrowser(this); // 属性浏览类
    d->m_treeBrowser->setRootIsDecorated(false);
    d->m_treeBrowser->setPropertiesWithoutValueMarked(true);
    d->m_treeBrowser->setResizeMode(QtTreePropertyBrowser::Interactive);
    // 定义一个FactoryManger
    d->m_propertyManager = new qdesigner_internal::DesignerPropertyManager(this);
    d->m_treeFactory = new qdesigner_internal::DesignerEditorFactory(this);

    QtVariantPropertyManager *variantManager = d->m_propertyManager;
    d->m_treeBrowser->setFactoryForManager(variantManager, d->m_treeFactory); // 设置属性管理器和属性编辑控件库
    d->m_treeBrowser->setSplitterPosition(150);

    d->m_filterWidget->setPlaceholderText(tr("Filter"));
    d->m_filterWidget->setClearButtonEnabled(true);

    connect(d->m_filterWidget, &QLineEdit::textChanged, d, &CamyuPropertyEditPrivate::setFilter);
    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->setMargin(0);
    vlayout->addWidget(d->m_filterWidget);
    vlayout->addWidget(d->m_treeBrowser);
    setLayout(vlayout);
    connect(d->m_treeFactory, &qdesigner_internal::DesignerEditorFactory::resetProperty,
        d, &CamyuPropertyEditPrivate::slotResetProperty);
    connect(d->m_propertyManager, &qdesigner_internal::DesignerPropertyManager::valueChanged,
        d, &CamyuPropertyEditPrivate::slotValueChanged);
}

CamyuPropertyEdit::~CamyuPropertyEdit()
{
    if (d)
    {
        delete d;
        d = 0;
    }
}
}
#include "CamyuPropertyEdit.moc"
