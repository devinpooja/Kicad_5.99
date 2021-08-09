/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2018 Jean-Pierre Charras jp.charras at wanadoo.fr
 * Copyright (C) 1992-2021 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef PCB_SHAPE_H
#define PCB_SHAPE_H

#include <board_item.h>
#include <eda_units.h>
#include <convert_to_biu.h>
#include <math_for_graphics.h>
#include <trigo.h>
#include <geometry/shape_poly_set.h>


class LINE_READER;
class EDA_DRAW_FRAME;
class FOOTPRINT;
class MSG_PANEL_ITEM;


class PCB_SHAPE : public BOARD_ITEM
{
public:
    PCB_SHAPE( BOARD_ITEM* aParent = nullptr, KICAD_T idtype = PCB_SHAPE_T );

    // Do not create a copy constructor & operator=.
    // The ones generated by the compiler are adequate.

    ~PCB_SHAPE();

    static inline bool ClassOf( const EDA_ITEM* aItem )
    {
        return aItem && PCB_SHAPE_T == aItem->Type();
    }

    bool IsType( const KICAD_T aScanTypes[] ) const override
    {
        if( BOARD_ITEM::IsType( aScanTypes ) )
            return true;

        for( const KICAD_T* p = aScanTypes; *p != EOT; ++p )
        {
            if( *p == PCB_LOCATE_GRAPHIC_T )
                return true;
            else if( *p == PCB_LOCATE_BOARD_EDGE_T )
                return m_layer == Edge_Cuts;
        }

        return false;
    }

    void SetFilled( bool aFlag ) { m_filled = aFlag; }

    bool IsFilled() const
    {
        switch( m_shape )
        {
        case SHAPE_T::RECT:
        case SHAPE_T::CIRCLE:
        case SHAPE_T::POLY:
            return m_filled;

        case SHAPE_T::SEGMENT:
        case SHAPE_T::ARC:
        case SHAPE_T::BEZIER:
            return false;

        case SHAPE_T::LAST: // Make CLang compiler happy
            return false;
        }

        return false;       // Make GCC compiler happy
    }

    void SetWidth( int aWidth ) { m_width = aWidth; }
    int GetWidth() const        { return m_width; }

    /**
     * Set the angle for arcs, and normalizes it within the range 0 - 360 degrees.
     *
     * @param aAngle is tenths of degrees, but will soon be degrees.
     * @param aUpdateEnd set to true to update also arc end coordinates m_thirdPoint,
     *        so must be called after setting m_Start and m_End.
     */
    virtual void SetAngle( double aAngle, bool aUpdateEnd = true );
    double GetAngle() const { return m_angle; }

    void SetShape( SHAPE_T aShape )                 { m_shape = aShape; }
    SHAPE_T GetShape() const                        { return m_shape; }

    void SetBezierC1( const wxPoint& aPoint )       { m_bezierC1 = aPoint; }
    const wxPoint& GetBezierC1() const              { return m_bezierC1; }

    void SetBezierC2( const wxPoint& aPoint )       { m_bezierC2 = aPoint; }
    const wxPoint& GetBezierC2() const              { return m_bezierC2; }

    void SetPosition( const wxPoint& aPos ) override;
    wxPoint GetPosition() const override;

    /**
     * Return the starting point of the graphic.
     */
    const wxPoint& GetStart() const         { return m_start; }
    int GetStartY()                         { return m_start.y; }
    int GetStartX()                         { return m_start.x; }
    void SetStart( const wxPoint& aStart )  { m_start = aStart; }
    void SetStartY( int y )                 { m_start.y = y; }
    void SetStartX( int x )                 { m_start.x = x; }

    /**
     * Return the ending point of the graphic.
     */
    const wxPoint& GetEnd() const           { return m_end; }
    int GetEndY()                           { return m_end.y; }
    int GetEndX()                           { return m_end.x; }
    void SetEnd( const wxPoint& aEnd )      { m_end = aEnd; }
    void SetEndY( int y )                   { m_end.y = y; }
    void SetEndX( int x )                   { m_end.x = x; }

    /**
     * Return the third point of the graphic.
     */
    const wxPoint& GetThirdPoint() const           { return m_thirdPoint; }
    int GetThirdPointY()                           { return m_thirdPoint.y; }
    int GetThirdPointX()                           { return m_thirdPoint.x; }
    void SetThirdPoint( const wxPoint& aPoint )    { m_thirdPoint = aPoint; }
    void SetThirdPointY( int y )                   { m_thirdPoint.y = y; }
    void SetThirdPointX( int x )                   { m_thirdPoint.x = x; }

    // Some attributes are read only, since they are "calculated" from
    // m_Start, m_End, and m_Angle.
    // No Set...() function for these attributes.

    wxPoint GetCenter() const override;
    wxPoint GetArcStart() const      { return m_end; }
    wxPoint GetArcEnd() const;
    wxPoint GetArcMid() const;
    std::vector<wxPoint> GetRectCorners() const;

    /**
     * @return the angle of the starting point of this arc, between 0 and 3600 in 0.1 deg.
     */
    double GetArcAngleStart() const;

    /**
     * @return the angle of the ending point of this arc, between 0 and 3600 in 0.1 deg.
     */
    double GetArcAngleEnd() const;

    /**
     * Return the radius of this item.
     *
     * Has meaning only for arcs and circles.
     */
    int GetRadius() const;

    /**
     * Initialize the start arc point.
     *
     * Can be used for circles to initialize one point of the cicumference.
     */
    void SetArcStart( const wxPoint& aArcStartPoint )
    {
        m_end = aArcStartPoint;
    }

    /**
     * Initialize the end arc point.
     *
     * Can be used for circles to initialize one point of the cicumference.
     */
    void SetArcEnd( const wxPoint& aArcEndPoint )
    {
        m_thirdPoint = aArcEndPoint;
    }

    void SetCenter( const wxPoint& aCenterPoint ) { m_start = aCenterPoint; }

    /**
     * Set the three controlling points for an arc.
     *
     * NB: these are NOT what's currently stored, so we have to do some calculations behind
     * the scenes.  However, they are what SHOULD be stored.
     */
    void SetArcGeometry( const wxPoint& aStart, const wxPoint& aMid, const wxPoint& aEnd );

    /**
     * Allows items to return their visual center rather than their anchor.
     * For some shapes this is similar to GetPosition, but for polygonal shapes,
     * the anchor is not suitable (shows nothing): a point on the outline is better
     */
    const wxPoint GetFocusPosition() const override;

    /**
     * Return the parent footprint or NULL if PCB_SHAPE does not belong to a footprint.
     *
     * @return the parent footprint or NULL.
     */
    FOOTPRINT* GetParentFootprint() const;

    // Accessors:
    const std::vector<wxPoint>& GetBezierPoints() const { return m_bezierPoints; }

    /**
     * Build and return the list of corners in a std::vector<wxPoint>
     *
     * It must be used only to convert the SHAPE_POLY_SET internal corner buffer
     * to a list of wxPoints, and nothing else, because it duplicates the buffer,
     * that is inefficient to know for instance the corner count
     */
    const std::vector<wxPoint> BuildPolyPointsList() const;

    /**
     * @return the number of corners of the polygonal shape
     */
    int GetPointCount() const;

    // Accessors to the polygonal shape
    SHAPE_POLY_SET& GetPolyShape() { return m_poly; }
    const SHAPE_POLY_SET& GetPolyShape() const { return m_poly; }

    /**
     * @return true if the polygonal shape is valid (has more than 2 points)
     */
    bool IsPolyShapeValid() const;

    void SetPolyShape( const SHAPE_POLY_SET& aShape ) { m_poly = aShape; }

    void SetBezierPoints( const std::vector<wxPoint>& aPoints )
    {
        m_bezierPoints = aPoints;
    }

    /**
     * Rebuild the m_BezierPoints vertex list that approximate the Bezier curve
     * by a list of segments.
     *
     * Has meaning only for S_CURVE DRAW_SEGMENT shape.
     *
     * @param aMinSegLen is the min length of segments approximating the he.  shape last segment
     *        can be shorter.  This parameter avoids having too many very short segment in list.
     *        A good value is m_Width/2 to m_Width.
     */
    void RebuildBezierToSegmentsPointsList( int aMinSegLen );

    void SetPolyPoints( const std::vector<wxPoint>& aPoints );

    /**
     * Make a set of SHAPE objects representing the PCB_SHAPE.  Caller owns the objects.
     */
    std::vector<SHAPE*> MakeEffectiveShapes() const; // fixme: move to shape_compound
    std::shared_ptr<SHAPE> GetEffectiveShape( PCB_LAYER_ID aLayer = UNDEFINED_LAYER ) const override;

    void GetMsgPanelInfo( EDA_DRAW_FRAME* aFrame, std::vector<MSG_PANEL_ITEM>& aList ) override;

    const EDA_RECT GetBoundingBox() const override;

    bool HitTest( const wxPoint& aPosition, int aAccuracy = 0 ) const override;
    bool HitTest( const EDA_RECT& aRect, bool aContained, int aAccuracy = 0 ) const override;

    wxString GetClass() const override
    {
        return wxT( "PCB_SHAPE" );
    }

    /**
     * Return the length of the track using the hypotenuse calculation.
     *
     * @return the length of the track
     */
    double GetLength() const;

    virtual void Move( const wxPoint& aMoveVector ) override;

    virtual void Rotate( const wxPoint& aRotCentre, double aAngle ) override;

    virtual void Flip( const wxPoint& aCentre, bool aFlipLeftRight ) override;

    void Scale( double aScale );

    /**
     * Convert the draw segment to a closed polygon.
     *
     * Used in filling zones calculations.  Circles and arcs are approximated by segments.
     *
     * @param aCornerBuffer is a buffer to store the polygon.
     * @param aClearanceValue is the clearance around the pad.
     * @param aError is the maximum deviation from a true arc.
     * @param ignoreLineWidth is used for edge cut items where the line width is only
     *        for visualization
     */
    void TransformShapeWithClearanceToPolygon( SHAPE_POLY_SET& aCornerBuffer,
                                               PCB_LAYER_ID aLayer, int aClearanceValue,
                                               int aError, ERROR_LOC aErrorLoc,
                                               bool ignoreLineWidth = false ) const override;

    virtual wxString GetSelectMenuText( EDA_UNITS aUnits ) const override;

    virtual BITMAPS GetMenuImage() const override;

    virtual EDA_ITEM* Clone() const override;

    virtual const BOX2I ViewBBox() const override;

    virtual void SwapData( BOARD_ITEM* aImage ) override;

    struct cmp_drawings
    {
        bool operator()( const BOARD_ITEM* aFirst, const BOARD_ITEM* aSecond ) const;
    };

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif

protected:
    // Compute the bounding box for an arc
    void computeArcBBox( EDA_RECT& aBBox ) const;

    const std::vector<wxPoint> buildBezierToSegmentsPointsList( int aMinSegLen  ) const;

    int                  m_width;        // thickness of lines ...
    bool                 m_filled;       // Pretty much what it says on the tin...
    wxPoint              m_start;        // Line start point or Circle and Arc center
    wxPoint              m_end;          // Line end point or circle and arc start point
    wxPoint              m_thirdPoint;   // Used only for Arcs: arc end point

    SHAPE_T       m_shape;        // Shape: line, Circle, Arc
    double               m_angle;        // Used only for Arcs: Arc angle in 1/10 deg
    wxPoint              m_bezierC1;     // Bezier Control Point 1
    wxPoint              m_bezierC2;     // Bezier Control Point 2

    std::vector<wxPoint> m_bezierPoints;
    SHAPE_POLY_SET       m_poly;         // Stores the S_POLYGON shape
};

#endif  // PCB_SHAPE_H
