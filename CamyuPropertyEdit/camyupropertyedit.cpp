#include "camyupropertyedit.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHash>
#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QThread>
#include <QLibrary>
#include <QMessageBox>

#include <cycore/cycamera.h>
#include <cycore/cycamerafactory.h>


#include "GenICamVersion.h"
#include "GenICamFwd.h"
#include "GenICam.h"

#include "MytestIPort.h"

#include "QtTreePropertyBrowser.h"
#include "designerpropertymanager.h"

typedef void(*AD9928_FUNC)(uint, uint, uint);
typedef void(*SetAd9928Callback)(AD9928_FUNC);
typedef void(*ShowAd9928)(void *);
typedef void(*DeleteAD9928)();
//回调函数
//                  0X166  
void ProcessAD9928(uint cmd, uint Addr, uint Param)
{

}
using namespace GenICam;
using namespace GenApi;

namespace CAMYUPROPEDIT{
class CamyuPropertyEditPrivate : public QObject
{
    Q_OBJECT
public:
    CamyuPropertyEditPrivate()
    {
        QLibrary libcheck("AD9928");
        if (libcheck.load())
        {
            ShowAd9928_sys = (ShowAd9928)libcheck.resolve("ShowAd9928");
            DeleteAD9928_sys = (DeleteAD9928)libcheck.resolve("DeleteAD9928");
            SetAd9928Callback_sys = (SetAd9928Callback)libcheck.resolve("SetAd9928Callback");
            if (SetAd9928Callback_sys)
            {
                SetAd9928Callback_sys(ProcessAD9928);
            }
        }
    }
    QLineEdit *m_filterWidget = nullptr;
    QtTreePropertyBrowser * m_treeBrowser = nullptr;
    qdesigner_internal::DesignerPropertyManager *m_propertyManager = nullptr;
    qdesigner_internal::DesignerEditorFactory *m_treeFactory = nullptr;
    QtVariantProperty * m_Property = nullptr;

    CNodeMapRef m_nodeMap;      // GenICam xml解析器

    QString m_filterPattern;
    QHash<QString, QtVariantProperty*> m_nameToProperty;
    CYCore::CYCamera * m_pCamera = nullptr;
    CYCore::CYCameraFactory * m_pCameraFactory = nullptr;

    MytestIPort m_port;

    ShowAd9928 ShowAd9928_sys = 0;
    DeleteAD9928 DeleteAD9928_sys = 0;
    SetAd9928Callback SetAd9928Callback_sys = 0;
public:
    // 设置界面的值
    void nodeCallBack(INode *pNode)
    {
        printf("Node : %s is changed\n", pNode->GetDisplayName().c_str());
        try
        {
            const QString displayName = pNode->GetDisplayName().c_str();
            const QString propertyName = pNode->GetName().c_str();
            QtVariantProperty * property = m_nameToProperty.value(propertyName, 0);
            QVariant pValue;
            if (!property)
            {
                qDebug("Node : %s Not Found!!!\n", propertyName.toStdString().c_str());
                return;
            }
            switch (pNode->GetPrincipalInterfaceType())
            {
            case intfIInteger: // inttype
            {
                CIntegerPtr pProperty = pNode;
                if (IsReadable(pNode))
                {
                    int intVal = pProperty->GetValue();
                    pValue = intVal;
                }
                else
                {
                    pValue = property->value();
                }
                int intMaxVal = pProperty->GetMax();intMaxVal = intMaxVal < 0 ? 0x7FFFFFFF : intMaxVal;
                int intMinVal = pProperty->GetMin();intMinVal = intMinVal > intMaxVal ? intMaxVal : intMinVal;
                property->setAttribute("maximum", intMaxVal);
                property->setAttribute("minimum", intMinVal);
                property->setAttribute("singleStep", pProperty->GetInc());
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            case intfIBoolean: // bool
            {
                CBooleanPtr pProperty = pNode;
                if (IsReadable(pNode))
                    pValue = pProperty->GetValue();
                else
                {
                    pValue = property->value();
                }
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            case intfIFloat:
            {
                CFloatPtr pProperty = pNode;
                if (IsReadable(pNode))
                    pValue = pProperty->GetValue();
                else
                {
                    pValue = property->value();
                }
                property->setAttribute("maximum", pProperty->GetMax());
                property->setAttribute("minimum", pProperty->GetMin());
                if (pProperty->HasInc())
                {
                    property->setAttribute("singleStep", pProperty->GetInc());
                }
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            case intfIString:
            {
                CStringPtr pProperty = pNode;
                if (IsReadable(pNode))
                    pValue = pProperty->GetValue().c_str();
                else
                {
                    pValue = property->value();
                }
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            case intfIEnumeration:
            {
                CEnumerationPtr pProperty = pNode;
                QStringList names;
                NodeList_t pEntries;
                pProperty->GetEntries(pEntries);
                for (auto pVar : pEntries)
                {
                    CEnumEntryPtr pEntry = pVar;
                    CNodePtr pnd = pVar;
                    /*pEntry->GetSymbolic().c_str();*/
                    names.append(pnd->GetDisplayName().c_str());
                }
                if (IsReadable(pNode))
                    pValue = pProperty->GetIntValue();
                else
                {
                    pValue = property->value();
                }
                property->setAttribute("enumNames", names);
                property->setValue(pValue);
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            default:
            {
                pValue = GetInterfaceName(pNode).c_str();
                property->setEnabled(false);
            }
            break;
            }
            property->setValue(pValue);
        }
        catch (GenICam::GenericException &e)
        {
            QMessageBox::warning(0, QStringLiteral("参数设置错误!!!"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
        }
        return;
    }
    void showMFCdlg()
    {
        if (ShowAd9928_sys)
        {
            ShowAd9928_sys(0);
        }
    }
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
        if (nameKey.isEmpty())
        {
            return;
        }
        CNodePtr pNode = m_nodeMap._GetNode(nameKey.toStdString().c_str());
        QVariant pValue;
        if (!pNode)
        {
            return;
        }
        switch (pNode->GetPrincipalInterfaceType())
        {
        case intfIInteger: // inttype
        {
            CIntegerPtr pProperty = pNode;
            if (IsWritable(pNode))
            {
                int intVal = varProp->value().toInt();
                pProperty->SetValue(intVal);
            }
        }
        break;
        case intfIBoolean: // bool
        {
            CBooleanPtr pProperty = pNode;
            if (IsWritable(pNode))
            {
                bool boolVal = varProp->value().toBool();
                pProperty->SetValue(boolVal);
            }
        }
        break;
        case intfIFloat:
        {
            CFloatPtr pProperty = pNode;
            if (IsWritable(pNode))
            {
                float floatVal = varProp->value().toFloat();
                pProperty->SetValue(floatVal);
            }
        }
        break;
        case intfIString:
        {
            CStringPtr pProperty = pNode;
            if (IsWritable(pNode))
            {
                QString stringVal = varProp->value().toString();
                pProperty->SetValue(stringVal.toStdString().c_str());
            }
        }
        break;
        case intfIEnumeration:
        {
            CEnumerationPtr pProperty = pNode;
            if (IsWritable(pNode))
            {
                int val = varProp->value().toInt();
                const QString valName = varProp->attributeValue("enumNames").toStringList().at(val);

                NodeList_t pEntries;
                CEnumEntryPtr pEntry;
                pProperty->GetEntries(pEntries);
                for (auto pVar : pEntries)
                {
                    CNodePtr pnd = pVar;
                    QString entryName = pnd->GetDisplayName().c_str();
                    if (entryName == valName)
                    {
                        pEntry = pVar;
                        break;
                    }
                }

                CEnumEntryPtr enumEntry = pEntry;//pProperty->GetEntryByName(valName.toStdString().c_str());
                if (enumEntry)
                {
                    pProperty->SetIntValue(enumEntry->GetValue());
                }
                //pProperty->SetValue(floatVal);
            }
        }
        break;
        default:
        {
            //pValue = GetInterfaceName(pNode).c_str();
            //property = m_propertyManager->addProperty(pValue.type(), displayName);
            //property->setEnabled(/*IsAvailable(pNode)*/false);
        }
        break;
        }
    }
    bool loadXMLFromFile(QString xmlPath)
    {
        try
        {
            m_nodeMap._LoadXMLFromFile(xmlPath.toStdString().c_str());
            m_nodeMap._Connect(&m_port);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
            QMessageBox::warning(0, QStringLiteral("XML解析"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
        }
        return false;
    }
    bool genIcamGetNode(char * nodename,INode *& pNode)
    {
        try {
            pNode = m_nodeMap._GetNode(nodename);
            return true;
        }
        catch (GenICam::GenericException &e) {
            QMessageBox::warning(0, QStringLiteral("获取节点"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
            pNode = 0;
        }
        return false;
    }
    bool genIcamCategoryParse(CCategoryPtr pCategory, QtVariantProperty * groupProperty)
    {
        try
        {
            QtVariantProperty *sub_groupProperty = 0;
            CNodePtr pNode = pCategory;
            const QString propertyName = pNode->GetName().c_str();
            sub_groupProperty = m_propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), pNode->GetDisplayName().c_str());
            m_nameToProperty.insert(propertyName, sub_groupProperty);
            // 首先向列表控件添加一个目录
            if (groupProperty)
            {
                groupProperty->addSubProperty(sub_groupProperty);
            }
            else
            {
                m_treeBrowser->addProperty(sub_groupProperty);
            }
            // 然后再循环解析目录下的子节点
            FeatureList_t featureList;
            pCategory->GetFeatures(featureList);
            for (auto feature : featureList)
            {
                INode * pNode = feature->GetNode();
                genIcamParaseINode(pNode, sub_groupProperty);
            }
        }
        catch (GenICam::GenericException &e)
        {
            QMessageBox::warning(0, QStringLiteral("解析目录"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
        }
        return false;
    }
    bool genIcamParaseCNode(CNodePtr pNode, QtVariantProperty * groupProperty)
    {
        try
        {
            EInterfaceType nodeType = pNode->GetPrincipalInterfaceType();
            EVisibility    eVty = pNode->GetVisibility();
            EAccessMode eAm = pNode->GetAccessMode();
            const QString displayName = pNode->GetDisplayName().c_str();
            const QString propertyName = pNode->GetName().c_str();

            QVariant pValue;
            QtVariantProperty *property = 0;
            Register(pNode, *this, &CamyuPropertyEditPrivate::nodeCallBack);
            switch (nodeType)
            {
            case GENAPI_NAMESPACE::intfIInteger:
            {
                // 处理int
                CIntegerPtr pProperty = pNode;
                if (IsReadable(pNode))
                {
                    int intVal = pProperty->GetValue();
                    pValue = intVal;
                }
                else
                {
                    pValue = 0;
                }
                int intMaxVal = pProperty->GetMax();intMaxVal = intMaxVal < 0 ? 0x7FFFFFFF : intMaxVal;
                int intMinVal = pProperty->GetMin();intMinVal = intMinVal > intMaxVal ? intMaxVal : intMinVal;
                int64_t pollt = pNode->GetPollingTime();
                property = m_propertyManager->addProperty(pValue.type(), displayName);
                property->setAttribute("maximum", /*pProperty->GetMax()*/intMaxVal);
                property->setAttribute("minimum", /*pProperty->GetMin()*/intMinVal);
                property->setAttribute("singleStep", pProperty->GetInc());
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            case GENAPI_NAMESPACE::intfIBoolean:
            {
                CBooleanPtr pProperty = pNode;
                if (IsReadable(pNode))
                    pValue = pProperty->GetValue();
                else
                {
                    pValue = false;
                }
                property = m_propertyManager->addProperty(pValue.type(), displayName);
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
//             case GENAPI_NAMESPACE::intfICommand:
//             {
//                 // 处理int
//             }
//             break;
            case GENAPI_NAMESPACE::intfIFloat:
            {
                CFloatPtr pProperty = pNode;
                if (IsReadable(pNode))
                    pValue = pProperty->GetValue();
                else
                {
                    pValue = 0.01;
                }
                property = m_propertyManager->addProperty(pValue.type(), displayName);
                property->setAttribute("maximum", pProperty->GetMax());
                property->setAttribute("minimum", pProperty->GetMin());
                if (pProperty->HasInc())
                {
                    property->setAttribute("singleStep", pProperty->GetInc());
                }
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            case GENAPI_NAMESPACE::intfIString:
            {
                CStringPtr pProperty = pNode;
                if (IsReadable(pNode))
                    pValue = pProperty->GetValue().c_str();
                else
                {
                    pValue = "";
                }
                property = m_propertyManager->addProperty(pValue.type(), displayName);
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
//             case GENAPI_NAMESPACE::intfIRegister:
//             {
//                 // 处理int
//             }
//             break;
            case GENAPI_NAMESPACE::intfIEnumeration:
            {
                CEnumerationPtr pProperty = pNode;
                property = m_propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), displayName);
                QStringList names;
                NodeList_t pEntries;
                pProperty->GetEntries(pEntries);
                for (auto pVar : pEntries)
                {
                    CEnumEntryPtr pEntry = pVar;
                    CNodePtr pnd = pVar;
                    /*pEntry->GetSymbolic().c_str();*/
                    names.append(pnd->GetDisplayName().c_str());
                }
                if (IsReadable(pNode))
                    pValue = pProperty->GetIntValue();
                else
                {
                    pValue = 0;
                }
                property->setValueToolTip("13");
                property->setAttribute("enumNames", names);
                property->setValue(pValue);
                property->setEnabled(IsAvailable(pNode) && IsWritable(pNode) && IsImplemented(pNode));
            }
            break;
            default:
            {
                pValue = GetInterfaceName(pNode).c_str();
                property = m_propertyManager->addProperty(pValue.type(), displayName);
                property->setEnabled(/*IsAvailable(pNode)*/false);
            }
                break;
            }
            property->setValue(pValue);
            if (!groupProperty)
            {
                m_treeBrowser->addProperty(property);
            }
            else
            {
                groupProperty->addSubProperty(property);
            }
            m_nameToProperty.insert(propertyName, property);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
            QMessageBox::warning(0, QStringLiteral("解析节点"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
        }
        return false;
    }
    bool genIcamParaseINode(INode * pNode, QtVariantProperty * groupProperty)
    {
        if (!pNode)
        {
            return false;
        }
        // 获取节点类型
        EInterfaceType nodeType = pNode->GetPrincipalInterfaceType();
        switch (nodeType)
        {
        case GENAPI_NAMESPACE::intfICategory:
        {
            genIcamCategoryParse(pNode, groupProperty);
        }
        break;
        default:
        {
            genIcamParaseCNode(pNode, groupProperty);
        }
            break;
        }
    }
    int parseGenICamXML(QString xmlPath)
    {
        if (loadXMLFromFile(xmlPath))
        {
            INode * pNode = 0;
            if (genIcamGetNode("Root",pNode))
            {
                genIcamParaseINode(pNode,0);
            }
        }
        return 0;
    }

    // cycamera slot
public slots:
    void cameraConnectChange(int chl)
    {
        if (!(m_pCamera && m_pCameraFactory))
        {
            return;
        }
        if (m_pCamera->isConnect(chl))
        {
            // 连接成功，状态改变为连接成功
            // 读取制定目录下的
            QString strbase = QCoreApplication::applicationDirPath();
            QString strGenIcamXmlPath = strbase + "/genicam/xml/" + m_pCameraFactory->category()+"/"+m_pCameraFactory->name() + ".xml";
            // 以下解析xml
            QtProperty *lastProperty = 0;
            QtVariantProperty *groupProperty = 0;
            QtVariantProperty *_groupProperty = 0;
            QtVariantProperty *property = 0;
            QVariant pValue;
            const QString displayName = "name";
            const QString propertyName = "CameraMessage";
            groupProperty = m_propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), propertyName);
            m_treeBrowser->addProperty(groupProperty);

            // 分组
            pValue = m_pCameraFactory->category();
            property = m_propertyManager->addProperty(pValue.type(), "category");
            property->setValue(pValue);
            property->setEnabled(false);
            groupProperty->addSubProperty(property);
            // 名字
            pValue = m_pCameraFactory->name();
            property = m_propertyManager->addProperty(pValue.type(), "name");
            property->setValue(pValue);
            groupProperty->addSubProperty(property);
            property->setEnabled(false);
            
            parseGenICamXML(strGenIcamXmlPath);
        }
        else
        {
            // 连接失败，状态转变为连接失败，未连接
            m_treeBrowser->clear();
            m_propertyManager->clear();
            m_nameToProperty.clear();
            m_nodeMap._ClearXMLCache();
            m_nodeMap._Destroy();
        }
    }
    void cameraCaptureChange(int chl)
    {
        
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
    vlayout->setMargin(1);
    vlayout->setSpacing(1);

    // 添加AD9928的启动按钮
    QPushButton * pushAd9928 = new QPushButton(tr("AD9928"));
    //pushAd9928->setCheckable(true);
    QObject::connect(pushAd9928, &QAbstractButton::clicked, this,[this]() {
        d->showMFCdlg();
    });
    vlayout->addWidget(pushAd9928);
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

void CamyuPropertyEdit::bindCamera(CYCore::CYCamera*pCamera)
{
    d->m_pCamera = pCamera;
    QObject::connect(pCamera, &CYCore::CYCamera::sigConnectChange, d, &CamyuPropertyEditPrivate::cameraConnectChange);
    QObject::connect(pCamera, &CYCore::CYCamera::sigCaptureChange, d, &CamyuPropertyEditPrivate::cameraCaptureChange);
}
void CamyuPropertyEdit::bindCameraFactory(CYCore::CYCameraFactory * pfactory)
{
    d->m_pCameraFactory = pfactory;
}
}
#include "CamyuPropertyEdit.moc"
