#include "camyupropertyedit.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHash>
#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>

#include <cycore/cycamera.h>
#include <cycore/cycamerafactory.h>


#include "GenICamVersion.h"
#include "GenICamFwd.h"
#include "GenICam.h"


#include "QtTreePropertyBrowser.h"
#include "designerpropertymanager.h"

using namespace GenICam;
using namespace GenApi;

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

    CNodeMapRef m_nodeMap;      // GenICam xml������

    QString m_filterPattern;
    QHash<QString, QtVariantProperty*> m_nameToProperty;
    CYCore::CYCamera * m_pCamera = nullptr;
    CYCore::CYCameraFactory * m_pCameraFactory = nullptr;
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
    bool loadXMLFromFile(QString xmlPath)
    {
        try
        {
            m_nodeMap._LoadXMLFromFile(xmlPath.toStdString().c_str());
            return true;
        }
        catch (GenICam::GenericException &e)
        {
            QMessageBox::warning(0, QStringLiteral("XML����"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
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
            QMessageBox::warning(0, QStringLiteral("��ȡ�ڵ�"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
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
            sub_groupProperty = m_propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), pNode->GetDisplayName().c_str());
            // �������б�ؼ����һ��Ŀ¼
            if (groupProperty)
            {
                groupProperty->addSubProperty(sub_groupProperty);
            }
            else
            {
                m_treeBrowser->addProperty(sub_groupProperty);
            }
            // Ȼ����ѭ������Ŀ¼�µ��ӽڵ�
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
            QMessageBox::warning(0, QStringLiteral("����Ŀ¼"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
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
            switch (nodeType)
            {
            case GENAPI_NAMESPACE::intfIInteger:
            {
                // ����int
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
//                 // ����int
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
//                 // ����int
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
            return true;
        }
        catch (GenICam::GenericException &e)
        {
            QMessageBox::warning(0, QStringLiteral("�����ڵ�"), e.what(), QMessageBox::Yes, QMessageBox::NoButton);
        }
        return false;
    }
    bool genIcamParaseINode(INode * pNode, QtVariantProperty * groupProperty)
    {
        if (!pNode)
        {
            return false;
        }
        // ��ȡ�ڵ�����
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
            // ���ӳɹ���״̬�ı�Ϊ���ӳɹ�
            // ��ȡ�ƶ�Ŀ¼�µ�
            QString strbase = QCoreApplication::applicationDirPath();
            QString strGenIcamXmlPath = strbase + "/genicam/xml/" + m_pCameraFactory->category()+"/"+m_pCameraFactory->name() + ".xml";
            // ���½���xml
            QtProperty *lastProperty = 0;
            QtVariantProperty *groupProperty = 0;
            QtVariantProperty *_groupProperty = 0;
            QtVariantProperty *property = 0;
            QVariant pValue;
            const QString displayName = "name";
            const QString propertyName = "CameraMessage";
            groupProperty = m_propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), propertyName);
            m_treeBrowser->addProperty(groupProperty);

            // ����
            pValue = m_pCameraFactory->category();
            property = m_propertyManager->addProperty(pValue.type(), "category");
            property->setValue(pValue);
            property->setEnabled(false);
            groupProperty->addSubProperty(property);
            // ����
            pValue = m_pCameraFactory->name();
            property = m_propertyManager->addProperty(pValue.type(), "name");
            property->setValue(pValue);
            groupProperty->addSubProperty(property);
            property->setEnabled(false);
            
            parseGenICamXML(strGenIcamXmlPath);
        }
        else
        {
            // ����ʧ�ܣ�״̬ת��Ϊ����ʧ�ܣ�δ����
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
    // ����ui
    d->m_filterWidget = new QLineEdit(this);    //������
    d->m_treeBrowser = new QtTreePropertyBrowser(this); // ���������
    d->m_treeBrowser->setRootIsDecorated(false);
    d->m_treeBrowser->setPropertiesWithoutValueMarked(true);
    d->m_treeBrowser->setResizeMode(QtTreePropertyBrowser::Interactive);
    // ����һ��FactoryManger
    d->m_propertyManager = new qdesigner_internal::DesignerPropertyManager(this);
    d->m_treeFactory = new qdesigner_internal::DesignerEditorFactory(this);

    QtVariantPropertyManager *variantManager = d->m_propertyManager;
    d->m_treeBrowser->setFactoryForManager(variantManager, d->m_treeFactory); // �������Թ����������Ա༭�ؼ���
    d->m_treeBrowser->setSplitterPosition(150);

    d->m_filterWidget->setPlaceholderText(tr("Filter"));
    d->m_filterWidget->setClearButtonEnabled(true);

    connect(d->m_filterWidget, &QLineEdit::textChanged, d, &CamyuPropertyEditPrivate::setFilter);
    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->setMargin(1);
    vlayout->setSpacing(1);
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
