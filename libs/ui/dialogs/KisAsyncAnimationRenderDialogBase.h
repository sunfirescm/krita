/*
 *  Copyright (c) 2017 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KISASYNCANIMATIONRENDERDIALOGBASE_H
#define KISASYNCANIMATIONRENDERDIALOGBASE_H

#include <QObject>
#include "kis_types.h"
#include "kritaui_export.h"

class KisTimeRange;
class KisAsyncAnimationRendererBase;
class KisViewManager;

/**
 * @brief KisAsyncAnimationRenderDialogBase is a special class for rendering multiple
 *        frames of the image and putting them somewhere (saving into a file or just
 *        pushing into an openGL cache)
 *
 * The class handles a lot of boilerplate code for you and optimizes regeneration of
 * the frames using multithreading, according to the user's settings. The responsibilities
 * of the class are the following:
 *
 * Rendering itself:
 *   - fetch the list of dirtly frames using calcDirtyFrames()
 *   - create some clones of the image according to the user's settings
 *     to facilitate multithreaded rendering and processing of the frames
 *   - if the user doesn't have anough RAM, the clones will not be created
 *     (the memory overhead is calculated using "projections" metric of the
 *      statistics server).
 *   - feed the images/threads with dirty frames until the all the frames
 *     are done
 *
 * Progress reporting:
 *   - if batchMode() is false, the user will see a progress dialog showing
 *     the current progress with estimate about total processing timer
 *   - the user can also cancel the regeneration by pressing Cancel button
 *
 * Usage Details:
 *   - one should implement two methods to make the rendering work:
 *     - calcDirtyFrames()
 *     - createRenderer(KisImageSP image)
 *   - these methids will be called on the start of the rendering
 */
class KRITAUI_EXPORT KisAsyncAnimationRenderDialogBase : public QObject
{
    Q_OBJECT
public:
    enum Result {
        RenderComplete,
        RenderCancelled,
        RenderFailed
    };

public:
    /**
     * @brief construct and initialize the dialog
     * @param actionTitle the first line of the status reports that the user sees in the dialog
     * @param image the image that will be as a source of frames. Make sure the image is *not*
     *              locked by the time regenerateRange() is called
     * @param busyWait the dialog will not show for the specified time (in milliseconds)
     */
    KisAsyncAnimationRenderDialogBase(const QString &actionTitle, KisImageSP image, int busyWait = 200);
    virtual ~KisAsyncAnimationRenderDialogBase();

    /**
     * @brief start generation of frames and (if not in batch mode) show the dialog
     *
     * The link to view manager is used to barrier lock with visual feedback in the
     * end of the operation
     */
    virtual Result regenerateRange(KisViewManager *viewManager);

    /**
     * Set area of image that will be regenerated. If \p roi is empty,
     * full area of the image is regenerated.
     */
    void setRegionOfInterest(const QRegion &roi);

    /**
     * @see setRegionOfInterest()
     */
    QRegion regionOfInterest() const;

    /**
     * @brief setting batch mode to true will prevent any dialogs or message boxes from
     *        showing on screen. Please take it into account that using batch mode prevents
     *        some potentially dangerous recovery execution paths (e.g. delete the existing
     *        frames in the destination folder). In such case the rendering will be stopped with
     *        RenderFailed result.
     */
    void setBatchMode(bool value);

    /**
     * @see setBatchMode
     */
    bool batchMode() const;

private Q_SLOTS:
    void slotFrameCompleted(int frame);
    void slotFrameCancelled(int frame);

    void slotCancelRegeneration();

private:
    void tryInitiateFrameRegeneration();
    void updateProgressLabel();
    void cancelProcessingImpl(bool isUserCancelled);

protected:
    /**
     * @brief returns a list of frames that should be regenerated by the dialog
     *
     * Called by the dialog in the very beginning of regenerateRange()
     */
    virtual QList<int> calcDirtyFrames() const = 0;

    /**
     * @brief create a renderer object linked to \p image
     *
     * Renderer are special objects that connect to the individual image signals,
     * react on them and fetch the final frames data
     *
     * @see KisAsyncAnimationRendererBase
     */
    virtual KisAsyncAnimationRendererBase* createRenderer(KisImageSP image) = 0;

    virtual void initializeRendererForFrame(KisAsyncAnimationRendererBase *renderer,
                                            KisImageSP image, int frame) = 0;

private:
    struct Private;
    const QScopedPointer<Private> m_d;
};

#endif // KISASYNCANIMATIONRENDERDIALOGBASE_H
