/* This file is part of the KDE project
 * Copyright (C) 2011 Casper Boemann <cbo@kogmbh.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KOTEXTLAYOUTROOTAREAPROVIDER_H
#define KOTEXTLAYOUTROOTAREAPROVIDER_H

#include "kotext_export.h"

class KoTextLayoutRootArea;
class KoTextDocumentLayout;

/**
 * When laying out text we need an area where upon the text will be placed.
 * A KoTextLayoutRootAreaProvider provides the layout process with such areas
 */
class KOTEXT_EXPORT KoTextLayoutRootAreaProvider
{
public:
    /// constructor
    explicit KoTextLayoutRootAreaProvider();
    virtual ~KoTextLayoutRootAreaProvider();

    // provide another root area
    virtual KoTextLayoutRootArea *provide(KoTextDocumentLayout *documentLayout) = 0;

    virtual void releaseAllAfter(KoTextLayoutRootArea *afterThis) = 0;
    
    virtual void doPostLayout(KoTextLayoutRootArea *rootArea) = 0;
};

#endif
