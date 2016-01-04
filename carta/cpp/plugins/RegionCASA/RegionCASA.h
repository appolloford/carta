/**
 * This plugin can read region formats that CASA supports.
 */
#pragma once

#include "CartaLib/IPlugin.h"
#include "casacore/casa/Quanta/Quantum.h"
#include "casacore/coordinates/Coordinates/CoordinateSystem.h"
#include "imageanalysis/Annotations/AnnotationBase.h"
#include <QObject>

namespace Carta {
    namespace Lib {
        class RegionInfo;
    }
}

class RegionCASA : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.cartaviewer.IPlugin")
    Q_INTERFACES( IPlugin)

public:

    /**
     * Constructor.
     */
    RegionCASA(QObject *parent = 0);
    virtual bool handleHook(BaseHook & hookData) override;
    virtual std::vector<HookId> getInitialHookList() override;

private:
    /**
     * Get a list of the corner points of a region in pixels.
     * @param corners - a list of corner points in world units.
     * @param csys - the coordinate system of the containing image.
     * @param directions - a list of MDirections for the image.
     * @return - a list of corner points of a region in pixels.
     */
    std::vector<std::pair<double,double> >
        _getPixelVertices( const casa::AnnotationBase::Direction& corners,
            const casa::CoordinateSystem& csys, const casa::Vector<casa::MDirection>& directions ) const;

    /**
     * Get a lists of x- and y- coordinates of the corner points of a region based on world
     * coordinates.
     * @param x - a list of the x-coordinates of corner points.
     * @param y - a list of the y-coordinates of corner points.
     * @param csys - the coordinate system of the containing image.
     * @param directions - a list of MDirections for the image.
     */
    void _getWorldVertices(std::vector<casa::Quantity>& x, std::vector<casa::Quantity>& y,
            const casa::CoordinateSystem& csys,
            const casa::Vector<casa::MDirection>& directions ) const;

    /**
     * Load one or more regions based on the name of a file specifying regions in CASA format
     * and an image that will contain the region.
     * @param fileName - path to a .crtf file specifying one or more regions in CASA format.
     * @param imagePtr - the image that will contain the region(s).
     * @return - a list containing draw information for the regions that were loaded.
     */
    std::vector< std::shared_ptr<Carta::Lib::RegionInfo> >
        _loadRegion(const QString & fileName, std::shared_ptr<Carta::Lib::Image::ImageInterface> imagePtr );
};
