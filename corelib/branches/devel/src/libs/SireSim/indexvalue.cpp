/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <QDomDocument>
#include <QPair>

#include "SireSim/indexvalue.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireSim;

/////////
///////// Implementation of Range
/////////

/** Construct a null range */
Range::Range() : _start(0), _end(0), _step(0)
{}

/** Construct a range that represents the single index 'index' */
Range::Range(int index) : _start(index), _end(index), _step(0)
{}

/** Construct a range that represents the range from start to stop,
    in jumps of 'step'. Note that this will automatically ensure that
    the jump has the right sign e.g. Range(10,1,1) would use a step of -1 */
Range::Range(int start, int stop, int step)
      : _start(start), _end(stop), _step(step)
{
    if (_start == _end)
        _step = 0;
        
    else if (_start == 0)
    {
        _start = _end;
        _step = 0;
    }
    else if (_end == 0)
    {
        _end = _start;
        _step = 0;
    }
    else if (_step == 0)
    {
        _step = 1;
    }
    else if (_step < 0)
    {
        _step = -_step;
    }
}

/** Construct a range from the passed string. The string has the format
    number                  = single index
    start..end              = a range from start to end, in steps of 1
    start..(step)..end      = a range from start to end, in steps of 'step'
    
    Note that spaces are ignored */
Range::Range(QString range)
{
    //remove all spaces and everything from the ends of the string
    QString parsed = range.trimmed().replace(" ","");
    
    if (parsed.isEmpty())
    {
        this->operator=( Range() );
        return;
    }
    
    QStringList words = parsed.split("..", QString::KeepEmptyParts);
    
    bool ok = true;
    int start = 0;
    int end = 0;
    int jump = 0;
    
    if (words.count() > 3)
        ok = false;
                
    else if (words.count() == 1)
    {
        //this should just be the index
        start = words[0].toInt(&ok);
        
        if (ok)
        {
            end = start;
            jump = 0;
        }
    }
    else if (words.count() == 2)
    {
        //this should just be the start and the end, with step of 1
        start = words[0].toInt(&ok);
        
        if (ok)
        {
            if (words[1].trimmed().isEmpty())
            {
                end = -1;
                jump = 1;
            }
            else
            {
                end = words[1].toInt(&ok);
                if (ok)
                {
                    if (end == start)
                        jump = 0;
                    else
                        jump = 1;
                }
            }
        }
    }
    else
    {
        //this should be the start and the end, with the step
        start = words[0].toInt(&ok);
        
        if (ok)
        {
            if (words[2].trimmed().isEmpty())
            {
                end = -1;
            }
            else
                end = words[2].toInt(&ok);
            
            if (ok)
            {
                //the range should have the format "(number)"
                int left_idx = words[1].indexOf("(");
                int right_idx = words[1].indexOf(")");
                
                if (left_idx == -1 or right_idx == -1 or left_idx > right_idx)
                {
                    ok = false;
                }
                else
                {
                    jump = words[1].mid(left_idx+1, right_idx-left_idx-1).trimmed()
                                   .toInt(&ok);
                }
            }
        }
    }
    
    if (not ok)
        throw SireError::invalid_arg( QObject::tr(
                "Cannot interpret a range from \"%1\". It should have the "
                "format start..(step)..range").arg(range), CODELOC );
                
    this->operator=( Range(start,end,jump) );
}

/** Copy constructor */
Range::Range(const Range &other)
      : _start(other._start), _end(other._end), _step(other._step)
{}

/** Destructor */
Range::~Range()
{}

/** Copy assignment operator */
Range& Range::operator=(const Range &other)
{
    if (this != &other)
    {
        _start = other._start;
        _end = other._end;
        _step = other._step;
    }
    
    return *this;
}

/** Comparison operator */
bool Range::operator==(const Range &other) const
{
    return _start == other._start and _end == other._end 
              and _step == other._step;
}

/** Comparison operator */
bool Range::operator!=(const Range &other) const
{
    return not Range::operator==(other);
}

/** Return a string representation of the range */
QString Range::toString() const
{
    if (_start == _end)
    {
        return QString::number(_start);
    }
    else if (_step == 1)
    {
        return QString("%1..%2").arg(_start).arg(_end);
    }
    else
    {
        return QString("%1..(%2)..%3").arg(_start).arg(_step).arg(_end);
    }
}

QPair<int,int> getRange(int start, int end, int nvalues)
{
    if (start < 0)
        start = nvalues + 1 - start;
        
    if (end < 0)
        end = nvalues + 1 - end;
        
    if (start < 1)
        start = 1;
    
    if (start > nvalues)
        start = nvalues;
        
    if (end < 1)
        end = 1;
        
    if (end > nvalues)
        end = nvalues;
        
    return QPair<int,int>(start,end);
}

/** Return whether or not this range contains the value 'i',
    assuming that the total number of items is 'nvalues' */
bool Range::contains(int i, int nvalues) const
{
    if (_start == 0 or _end == 0 or nvalues == 0 or i == 0)
        return false;

    QPair<int,int> range = getRange(_start,_end,nvalues);
        
    if (i < 0)
        i = nvalues + 1 - i;
        
    if (i >= range.first and i <= range.second)
    {
        return (i-range.first) % _step == 0;
    }
    else
        return false;
}

/** Return the start value of the range */
int Range::start() const
{
    return _start;
}

/** Return the stop (end) value of the range */ 
int Range::stop() const
{
    return _end;
}

/** Return the jump between values in the range */
int Range::jump() const
{
    return _step;
}

/** Return the list of indicies associated with this range */
QList<int> Range::indicies(int nvalues) const
{
    if (_start == 0 or _end == 0)
        return QList<int>();
    else if (_step == 0)
    {
        QList<int> ret;
        ret.append(_start);
        return ret;
    }
    else
    {
        QPair<int,int> range = getRange(_start, _end, nvalues);
        QList<int> ret;
    
        if (range.first <= range.second)
        {
            for (int i=range.first; i<=range.second; i+=_step)
            {
                ret.append(i);
            }
        }
        else
        {
            for (int i=range.first; i>=range.second; i-=_step)
            {
                ret.append(i);
            }
        }
        
        return ret;
    }
}

/** Return the number of indicies if the number of values is 'nvalues' */
int Range::count(int nvalues) const
{
    if (_start == 0 or _end == 0 or nvalues == 0)
        return 0;
        
    else if (_step == 0)
        return 1;
        
    else
    {
        QPair<int,int> range = getRange(_start,_end,nvalues);
    
        if (range.first <= range.second)
        {
            return 1 + (range.second - range.first) / _step;
        }
        else
        {
            return 1 + (range.first - range.second) / _step;
        }
    }
}

/** Return the number of indicies in the batch with index 'i' */
int Range::count(int i, int nvalues, int batchsize) const
{
    int nidxs = this->count(nvalues);
    
    if (nidxs == 0)
        return 0;
        
    if (batchsize <= 0)
        batchsize = 1;
        
    int nbatches = (nidxs / batchsize) + 1;
    
    if (i >= nbatches or i < -nbatches)
        return 0;
    
    else if (i != nbatches-1)
        return batchsize;
    
    else
    {
        //get the remainder
        return nidxs - batchsize*(nbatches-1);
    }
}

/** Return the number of batches needed to get all of the indicies,
    for a given number of values, and a given batch size */
int Range::nBatches(int nvalues, int batchsize) const
{
    int nidxs = this->count(nvalues);
    
    if (nidxs == 0)
        return 0;
    else
        return (nidxs / batchsize) + 1;
}

/** Return the list of indicies, but in batches of 'batchsize' indicies. */
QList<int> Range::indicies(int i, int nvalues, int batchsize) const
{
    if (_start == 0 or _end == 0)
        return QList<int>();
    
    else if (_step == 0)
    {
        QList<int> ret;
        
        if (i == 0)
            ret.append(_start);
            
        return ret;
    }
    else
    {
        QPair<int,int> range = getRange(_start,_end,nvalues);
        QList<int> ret;
        int nbatches = this->nBatches(nvalues, batchsize);

        if (i >= nbatches or i < -nbatches)
            return ret;
        
        if (i < 0)
            i = nbatches + i;
        
        if (range.first <= range.second)
        {
            int count = 0;
            for (int j=(range.first+i*batchsize); j<=range.second; j += _step)
            {
                count += 1;
                
                if (count > batchsize)
                    break;

                ret.append(j);
            } 
        }
        else
        {
            int count = 0;
            for (int j=(range.first-i*batchsize); j>=range.second; j -= _step)
            {
                count += 1;
                
                if (count > batchsize)
                    break;

                ret.append(j);
            }
        }
        
        return ret;
    }
}

/** Return the ith index in this range, given the passed number of values */
int Range::at(int i, int nvalues) const
{
    QPair<int,int> range = getRange(_start,_end,nvalues);
    
    int nidxs = this->count(nvalues);
    
    if (i >= nidxs or i < -nidxs)
        return 0;
    
    else if (range.first <= range.second)
    {
        return range.first + _step*i;
    }
    else
    {
        return range.first - _step*i;
    }
}
