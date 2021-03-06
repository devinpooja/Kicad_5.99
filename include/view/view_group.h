/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * Copyright (C) 2020-2021 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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
 *
 */

/**
 * @file view_group.h
 */

#ifndef VIEW_GROUP_H_
#define VIEW_GROUP_H_

#include <view/view_item.h>
#include <deque>

namespace KIGFX
{
/**
 * Extend #VIEW_ITEM by possibility of grouping items into a single object.
 *
 * VIEW_GROUP does not take over ownership of the held items. The main purpose of this class is
 * to group items and draw them on a single layer (in particular the overlay).
 */
class VIEW_GROUP : public VIEW_ITEM
{
public:
    VIEW_GROUP( VIEW* aView = nullptr );
    virtual ~VIEW_GROUP();

    /**
     * Return the number of stored items.
     *
     * @return Number of stored items.
     */
    virtual unsigned int GetSize() const;

    /**
     * Add an item to the group.
     *
     * @param aItem is the item to be added.
     */
    virtual void Add( VIEW_ITEM* aItem );

    /**
     * Remove an item from the group.
     *
     * @param aItem is the item to be removed.
     */
    virtual void Remove( VIEW_ITEM* aItem );

    /**
     * Remove all the stored items from the group.
     */
    virtual void Clear();

    virtual VIEW_ITEM* GetItem( unsigned int aIdx ) const;

    /**
     * Return the bounding box for all stored items covering all its layers.
     *
     * @return The current bounding box
     */
    virtual const BOX2I ViewBBox() const override;

    /**
     * Draw all the stored items in the group on the given layer.
     *
     * @param aLayer is the layer which should be drawn.
     * @param aView is the VIEW that should be used for drawing.
     */
    virtual void ViewDraw( int aLayer, VIEW* aView ) const override;

    /**
     * Return all the layers used by the stored items.
     *
     * @param aLayers[] is the output layer index array.
     * @param aCount is the number of layer indices in aLayers[].
     */
    virtual void ViewGetLayers( int aLayers[], int& aCount ) const override;

    /**
     * Set layer used to draw the group.
     *
     * @param aLayer is the layer used for drawing.
     */
    inline virtual void SetLayer( int aLayer )
    {
        m_layer = aLayer;
    }

    /**
     * Free all the items that were added to the group.
     */
    void FreeItems();

protected:
    typedef std::vector<VIEW_ITEM*> ITEMS;

    virtual const ITEMS updateDrawList() const;

    ///< Layer on which the group is drawn.
    int m_layer;

    ///< Container for storing VIEW_ITEMs.
    ITEMS m_groupItems;
};

} // namespace KIGFX

#endif // VIEW_GROUP_H_
