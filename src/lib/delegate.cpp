/**
MIT License

Copyright (c) 2018 Michael Scopchanov
Copyright (c) 2022 Jihadist <tomasiche@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "delegate.h"
#include "common.h"
#include "delegate_p.h"
#include <QDateTime>
#include <QPainter>

Delegate::Delegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , m_ptr(new DelegatePrivate)
{
}

Delegate::~Delegate()
{
    delete m_ptr;
}

QMargins Delegate::contentsMargins() const
{
    return m_ptr->margins;
}

void Delegate::setContentsMargins(int left, int top, int right, int bottom)
{
    m_ptr->margins = QMargins(left, top, right, bottom);
}

int Delegate::horizontalSpacing() const
{
    return m_ptr->spacingHorizontal;
}

void Delegate::setHorizontalSpacing(int spacing)
{
    m_ptr->spacingHorizontal = spacing;
}

int Delegate::verticalSpacing() const
{
    return m_ptr->spacingVertical;
}

void Delegate::setVerticalSpacing(int spacing)
{
    m_ptr->spacingVertical = spacing;
}

void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QPalette& palette(opt.palette);
    const QRect& rect(opt.rect);
    const QRect& contentRect(rect.adjusted(m_ptr->margins.left(),
        m_ptr->margins.top(),
        -m_ptr->margins.right(),
        -m_ptr->margins.bottom()));
    const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
    const int bottomEdge = rect.bottom();

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);

    // Draw background
    painter->fillRect(rect, opt.state & QStyle::State_Selected ? palette.highlight().color() : palette.light().color());

    // Draw bottom line
    painter->setPen(lastIndex ? palette.dark().color()
                              : palette.mid().color());
    painter->drawLine(lastIndex ? rect.left() : m_ptr->margins.left(),
        bottomEdge, rect.right(), bottomEdge);

    painter->setFont(opt.font);
    // Draw timestamp
    QRect timeStampRect(m_ptr->boundedBox(opt, index));
    {
        timeStampRect.moveTo(m_ptr->margins.left(),
            contentRect.top());

        painter->setPen(palette.text().color());
        auto datetime = index.data(TimeRole).toDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
        painter->drawText(timeStampRect, Qt::TextSingleLine,
            QString("[%1]").arg(datetime));
    }
    //  Draw id
    QRect idRect(m_ptr->boundedBox(opt, index));
    {
        idRect.moveTo(timeStampRect.right() + m_ptr->spacingHorizontal, contentRect.top());
        auto id = QString::number(index.data(KeyRole).toInt());
        painter->setPen(palette.text().color());
        painter->drawText(idRect, Qt::TextSingleLine,
            QString("[%1]").arg(id));
    }
    // Draw message text
    QRect messageRect(m_ptr->messageBox(opt));
    {
        messageRect.moveTo(m_ptr->margins.left()
                + m_ptr->spacingHorizontal,
            timeStampRect.bottom() + m_ptr->spacingVertical);

        painter->setPen(palette.windowText().color());
        painter->drawText(messageRect, Qt::TextSingleLine, opt.text);
    }
    painter->restore();
}

QSize Delegate::sizeHint(const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int textHeight = m_ptr->boundedBox(opt, index).height()
        + m_ptr->spacingVertical + m_ptr->messageBox(opt).height();

    return QSize(opt.rect.width(), m_ptr->margins.top() + textHeight + m_ptr->margins.bottom());
}

DelegatePrivate::DelegatePrivate()
    : margins(0, 0, 0, 0)
    , spacingHorizontal(0)
    , spacingVertical(0)
{
}

QRect DelegatePrivate::boundedBox(const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    return QFontMetrics(option.font).boundingRect(QString("[%1]").arg(index.data(TimeRole).toString())).adjusted(0, 0, 1, 1);
}

QRect DelegatePrivate::messageBox(const QStyleOptionViewItem& option) const
{
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}
