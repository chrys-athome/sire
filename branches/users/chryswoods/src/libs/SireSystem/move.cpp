
#include "move.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireBase;
using namespace SireSim;
using namespace SireStream;

///////////
/////////// Implementation of MoveBase
///////////

static const RegisterMetaType<MoveBase> r_movebase(MAGIC_ONLY,
                                                   "SireSystem::MoveBase");

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const MoveBase&)
{
    writeHeader(ds, r_movebase, 0);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, MoveBase&)
{
    VersionID v = readHeader(ds, r_movebase);

    if (v != 0)
        throw version_error(v, "0", r_movebase, CODELOC);

    return ds;
}

/** Constructor */
MoveBase::MoveBase() : QSharedData()
{}

/** Copy constructor */
MoveBase::MoveBase(const MoveBase&)
         : QSharedData()
{}

/** Destructor */
MoveBase::~MoveBase()
{}

/** Assignment operator */
MoveBase& MoveBase::operator=(const MoveBase&)
{
    return *this;
}

///////////
/////////// Implementation of detail::NullMove
///////////

namespace detail
{

/** This is a null move, used to allow a functional, though
    ineffective null 'Move' object

    @author Christopher Woods
*/
class NullMove : public MoveBase
{
public:
    NullMove() : MoveBase()
    {}

    NullMove(const NullMove &other) : MoveBase(other)
    {}

    ~NullMove()
    {}

    NullMove& operator=(const NullMove &other)
    {
        MoveBase::operator=(other);
        return *this;
    }

    static const char* typeName()
    {
        return "SireSystem::detail::NullMove";
    }

    const char* what() const
    {
        return NullMove::typeName();
    }

    NullMove* clone() const
    {
        return new NullMove(*this);
    }

    void initialise(const SimSystem&)
    {}

    void move(SimSystem&)
    {}
};

}

Q_DECLARE_METATYPE(SireSystem::detail::NullMove);

static const RegisterMetaType<detail::NullMove> r_nullmove;

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const detail::NullMove &nullmove)
{
    writeHeader(ds, r_nullmove, 1)
        << static_cast<const MoveBase&>(nullmove);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream &operator>>(QDataStream &ds, detail::NullMove &nullmove)
{
    VersionID v = readHeader(ds, r_nullmove);

    if (v == 1)
    {
        ds >> static_cast<MoveBase&>(nullmove);
    }
    else
        throw version_error(v, "1", r_nullmove, CODELOC);

    return ds;
}

static SharedPolyPointer<MoveBase> shared_null( new NullMove() );

///////////
/////////// Implementation of Move
///////////

static const RegisterMetaType<Move> r_move;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const Move &move)
{
    writeHeader(ds, r_move, 1);

    SharedDataStream(ds) << move.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, Move &move)
{
    VersionID v = readHeader(ds, r_move);

    if (v == 1)
    {
        SharedDataStream(ds) >> move.d;
    }
    else
        throw version_error(v, "1", r_move, CODELOC);

    return ds;
}

/** Null constructor */
Move::Move() : d(shared_null)
{}

/** Construct from the passed move */
Move::Move(const MoveBase &move) : d(move)
{}

/** Construct from the passed pointer - this will be set to the
    null move if the pointer is null */
Move::Move(const SharedPolyPointer<MoveBase> &ptr) : d(ptr)
{
    if (!d)
        d = shared_null;
}

/** Copy constructor */
Move::Move(const Move &other) : d(other.d)
{}

/** Destructor */
Move::~Move()
{}

/** Assign from a move object */
Move& Move::operator=(const MoveBase &move)
{
    d = move;

    return *this;
}

/** Assign from a pointer - this will be set to the null object
    if the pointer is null */
Move& Move::operator=(const SharedPolyPointer<MoveBase> &ptr)
{
    if (ptr)
        d = ptr;
    else
        d = shared_null;

    return *this;
}

/** Copy assignment */
Move& Move::operator=(const Move &other)
{
    d = other;
    return *this;
}
