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
#include "objectselectorhelper.h"

#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QTimer>

using namespace GammaRay;

namespace {
    int m_maxTries = 10;
}

ObjectSelectorHelper::Selector::Selector(int r, const QVariant &o, Qt::MatchFlags mf)
    : role(r)
    , object(o)
    , matchFlags(mf)
{ }

ObjectSelectorHelper::ObjectSelectorHelper(QItemSelectionModel *selectionModel, QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_tries(0)
    , m_selectionModel(selectionModel)
{
    m_timer->setInterval(250);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
}

ObjectSelectorHelper::~ObjectSelectorHelper()
{
    m_timer->stop();
}

void ObjectSelectorHelper::timer_timeout()
{
    if (m_tries >= m_maxTries) {
        m_timer->stop();
    }
    else {
        ++m_tries;

        if (selectObject(m_pendingSelector)) {
            m_pendingSelector = Selector();
            m_timer->stop();
        }
    }
}

bool ObjectSelectorHelper::select(const Selector &selector)
{
    m_timer->stop();
    if (!selectObject(selector)) {
        m_tries = 0;
        m_pendingSelector = selector;
        m_timer->start();
        return false;
    }
    m_pendingSelector = Selector();
    return true;
}

bool ObjectSelectorHelper::select(int role, const QVariant &object, Qt::MatchFlags matchFlags)
{
    return select(Selector(role, object, matchFlags));
}

bool ObjectSelectorHelper::selectObject(const ObjectSelectorHelper::Selector &selector)
{
    const QAbstractItemModel *model = m_selectionModel->model();

    if (!model)
        return false;

    const QModelIndex startIndex = model->index(0, 0);
    const QModelIndexList indexes = model->match(startIndex, selector.role, selector.object, 1, selector.matchFlags);

    if (indexes.isEmpty()) {
        return false;
    }

    const QModelIndex index = indexes.first();
    m_selectionModel->select(index, QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);
    return true;
}
