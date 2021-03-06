/**
@file
Purpose: Lidar Detection

@date created 2010-06-03 16:13
@author Julien Moras
@author Sergio Rodriguez
@version $Id: $
*/

#include "ComputingComponent.h"

#include <boost/assign/std/vector.hpp>
#include <boost/current_function.hpp>
#include <cmath>
//#include <pcl/features/normal_3d_omp.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/filters/extract_indices.h>
//#include <pcl/filters/passthrough.h>
//#include <pcl/filters/statistical_outlier_removal.h>
//#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
//#include <pcl/kdtree/kdtree.h>
//#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
//#include <pcl/registration/transforms.h>
//#include <pcl/sample_consensus/method_types.h>
//#include <pcl/sample_consensus/model_types.h>
//#include <pcl/segmentation/extract_clusters.h>
//#include <pcl/segmentation/sac_segmentation.h>
#include <qmatrix.h>
#include <qtimer.h>

#include "kernel/ComponentFactory.h"
#include "kernel/Log.h"
#include "PacpusTools/ShMem.h"

#include "ui/widgetPCL.h"

using ::std::string;

using namespace pacpus;

DECLARE_STATIC_LOGGER("pacpus.cityvip.VelodynePCLViewer.ComputingComponent");

const char * ComputingComponent::COMPONENT_NAME = "ComputingComponent";
const char * ComputingComponent::COMPONENT_XML_NAME = "computingComponent";

/// Construct the factory
static ComponentFactory<ComputingComponent> sFactory(ComputingComponent::COMPONENT_NAME);

ComputingComponent::ComputingComponent(QString name)
    : ComponentBase(name)
{
    LOG_TRACE("constructor(" << name <<")");
}

ComputingComponent::~ComputingComponent()
{
    LOG_TRACE("destructor");
}

ComponentBase::COMPONENT_CONFIGURATION ComputingComponent::configureComponent(XmlComponentConfig /*config*/)
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);

    //Load Xml parameters

    LOG_INFO("configured component ComputingComponent");
    return ComponentBase::CONFIGURED_OK;
}

void ComputingComponent::startActivity()
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);

    startComponent();
}

void ComputingComponent::stopActivity()
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);

    stopComponent();
}

void ComputingComponent::startComponent()
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);

    // Positioning component searching
    ComponentManager * mgr = ComponentManager::getInstance();

    m_VelodyneInterface = static_cast<VelodyneInterface *>(mgr->getComponent("velodyneInterface"));
    m_VelodyneInterface->setVelodyneComputingStrategy(this);

    wi = new WidgetPCL();

    LOG_INFO("started component '" << componentName << "'");
}

void ComputingComponent::stopComponent()
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);

    delete wi;
    //PCL SAVE RESULTS

    string pcdAsciiFilePath = "test_pcd.pcd";
    pcl::io::savePCDFileASCII(pcdAsciiFilePath, globale_cloud);
    LOG_INFO("saved cloud with " << globale_cloud.points.size() << " data points to file '" << pcdAsciiFilePath.c_str() << "'");
    /*    for (size_t i = 0; i < cloud2.points.size (); ++i)
      std::cerr << "    " << cloud2.points[i].x << " " << cloud2.points[i].y << " " << cloud2.points[i].z << std::endl;
*/

    LOG_INFO("stopped component '" << componentName << "'");
}

void ComputingComponent::processRaw(VelodynePolarData * /*incomingData*/)
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);
}

void ComputingComponent::processCorrected(VelodyneCartData * incomingData)
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);
    //local run variables
    pcl::PointCloud<pcl::PointXYZ>::Ptr locale_cloud (new pcl::PointCloud<pcl::PointXYZ>);

    SetPointCloudFromScan(incomingData,locale_cloud);

    wi->updatePointCloud(locale_cloud->makeShared());
}

void ComputingComponent::SetPointCloudFromScan(VelodyneCartData * m_incomingData,pcl::PointCloud<pcl::PointXYZ>::Ptr locale_cloud)
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);

    int width=VELODYNE_SCAN_SIZE;
    int height=32;

    // Fill in the cloud data
    locale_cloud->width    = width;
    locale_cloud->height   = height;
    locale_cloud->is_dense = false;
    locale_cloud->points.resize (locale_cloud->width * locale_cloud->height);

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height ; ++j)
        {
            locale_cloud->points[i*height+j].x = m_incomingData->Data[i].Points[j].X;
            locale_cloud->points[i*height+j].y = m_incomingData->Data[i].Points[j].Y;
            locale_cloud->points[i*height+j].z = m_incomingData->Data[i].Points[j].Z;
            //qDebug() << "i" << i << "j" << j<< "index" <<i*height+j;
        }
    }
}

/*
void ComputingComponent::SetPointCloudFromScan(ScanAlascaData * m_incomingData,pcl::PointCloud<pcl::PointXYZ>::Ptr locale_cloud)
{
    LOG_TRACE(BOOST_CURRENT_FUNCTION);

    locale_cloud->width    = m_incomingData->nbPoint;
    locale_cloud->height   = 1;
    locale_cloud->is_dense = false;
    locale_cloud->points.resize (locale_cloud->width * locale_cloud->height);

    for (size_t i = 0; i < locale_cloud->points.size (); ++i)
    {
        locale_cloud->points[i].x = m_incomingData->point[i].x/100.0;
        locale_cloud->points[i].y = m_incomingData->point[i].y/100.0;
        locale_cloud->points[i].z = m_incomingData->point[i].z/100.0;
    }
}
*/
