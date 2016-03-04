/*
  objectselectorhelper.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Filipe Azevedo <filipe.azevedo@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  accordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
  @file
  This file is part of the GammaRay Plugin API and declares an helper class
  for asynchrone object selection.

  @brief
  Declares an helper class for asynchrone object selection.

  @author Filipe Azevedo \<filipe.azevedo@kdab.com\>
*/

#ifndef GAMMARAY_OBJECTSELECTORHELPER_H
#define GAMMARAY_OBJECTSELECTORHELPER_H

#include "gammaray_core_export.h"

#include <QObject>
#include <QVariant>

class QItemSelectionModel;
class QTimer;

namespace GammaRay {

/**
 * @brief An helper for selecting object asynchronously.
 */
class GAMMARAY_CORE_EXPORT ObjectSelectorHelper : public QObject
{
    Q_OBJECT

public:
    struct Selector
    {
        Selector(int role = -1, const QVariant &object = QVariant(), Qt::MatchFlags matchFlags = Qt::MatchExactly | Qt::MatchRecursive);

        int role;
        QVariant object;
        Qt::MatchFlags matchFlags;
    };

    explicit ObjectSelectorHelper(QItemSelectionModel *selectionModel, QObject *parent = 0);
    ~ObjectSelectorHelper();

    bool select(const Selector &selector);
    bool select(int role = -1, const QVariant &object = QVariant(), Qt::MatchFlags matchFlags = Qt::MatchExactly | Qt::MatchRecursive);

private slots:
    void timer_timeout();

private:
    QTimer *m_timer;
    int m_tries;
    QItemSelectionModel *m_selectionModel;
    Selector m_pendingSelector;

    bool selectObject(const Selector &selector);
};

}

#endif // GAMMARAY_OBJECTSELECTORHELPER_H
