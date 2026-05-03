//
// Adapted from:
// http://www.3dkingdoms.com/weekly/weekly.php?a=3
//
// The overall face-testing structure is inherited from the original
// CheckLineBox(B1, B2, L1, L2, Hit) snippet. GetIntersection() uses the
// same interpolation formula, just written as
//     bound2source / (bound2source - bound2target)
// instead of
//     -fDst1 / (fDst2 - fDst1)
// which is algebraically equivalent.
//
// Differences from the original snippet:
// - This version is adapted to the local geometry types FT, Point, Bbox, and
//   Segment, and exposes CheckSegmentBox(Bbox, Segment, Point&) instead of
//   CheckLineBox(B1, B2, L1, L2, Hit).
// - This implementation throws if either endpoint is exactly on a box plane.
//   The original implementation just returns 0 for that case.
// - This implementation returns false when the segment source is inside or on
//   the box. The original implementation returns true and sets Hit = L1 when
//   L1 is strictly inside the box.
// - The "source inside box" test here is inclusive, so points on the boundary
//   are treated as inside for the purpose of that early rejection.
// - This file also adds convenience helpers such as inside_bounds() and
//   box_contains_point(), which are not part of the original snippet.
class RayBoxIntersection
{
    public:

        bool
        inline
        GetIntersection( FT bound2source, FT bound2target, Point P1, Point P2, Point &Hit )
        {
            if( (bound2source == 0) || (bound2target == 0) ) // one or both are ON the boundary
            {
                throw std::runtime_error( "cannot handle this case yet" );
            }
            if( (bound2source * bound2target) > 0 ) return false; // source and target are on the same side of the boundary
            if( bound2source == bound2target ) return false; // source and target are aligned parallel to the boundary
            Hit = P1 + (P2-P1) * ( bound2source/(bound2source-bound2target) );
            return true;
        }

        bool
        inline
        InBox( Point Hit, Bbox box, const int Axis )
        {
            if( Axis==1 && Hit.z > box.zmin() && Hit.z < box.zmax() && Hit.y > box.ymin() && Hit.y < box.ymax() ) return true;
            if( Axis==2 && Hit.z > box.zmin() && Hit.z < box.zmax() && Hit.x > box.xmin() && Hit.x < box.xmax() ) return true;
            if( Axis==3 && Hit.x > box.xmin() && Hit.x < box.xmax() && Hit.y > box.ymin() && Hit.y < box.ymax() ) return true;
            return false;
        }

        // returns true if Segment intersects with the box
        // returns intersection point in Hit
        int
        CheckSegmentBox( Bbox box, Segment segment, Point &Hit )
        {
            Point source = segment.source();
            Point target = segment.target();

            // rule out segments that are entirely inside the box (ON EDGE = INSIDE)
            if( source.x() >= box.xmin() && source.x() <= box.xmax() &&
                source.y() >= box.ymin() && source.y() <= box.ymax() &&
                source.z() >= box.zmin() && source.z() <= box.zmax() )
            {
                return false;
            }

            // rule out segments that are entirely outside the box (SHOULD NEVER HAPPEN)
            if( ( target.x() < box.xmin() && source.x() < box.xmin() ) ||
                ( target.x() > box.xmax() && source.x() > box.xmax() ) ||
                ( target.y() < box.ymin() && source.y() < box.ymin() ) ||
                ( target.y() > box.ymax() && source.y() > box.ymax() ) ||
                ( target.z() < box.zmin() && source.z() < box.zmin() ) ||
                ( target.z() > box.zmax() && source.z() > box.zmax() ) )
            {
                return false;
            }

            // heavy lifting
            if ( ( GetIntersection( source.x()-box.xmin(), target.x()-box.xmin(), source, target, Hit ) && InBox( Hit, box, 1 ) ) ||
                ( GetIntersection( source.y()-box.ymin(), target.y()-box.ymin(), source, target, Hit ) && InBox( Hit, box, 2 ) ) ||
                ( GetIntersection( source.z()-box.zmin(), target.z()-box.zmin(), source, target, Hit ) && InBox( Hit, box, 3 ) ) ||
                ( GetIntersection( source.x()-box.xmax(), target.x()-box.xmax(), source, target, Hit ) && InBox( Hit, box, 1 ) ) ||
                ( GetIntersection( source.y()-box.ymax(), target.y()-box.ymax(), source, target, Hit ) && InBox( Hit, box, 2 ) ) ||
                ( GetIntersection( source.z()-box.zmax(), target.z()-box.zmax(), source, target, Hit ) && InBox( Hit, box, 3 ) ) )
            {
                return true;
            }
            return false;
        }

    public:

        inline
        bool
        inside_bounds( FT x, FT min, FT max )
        {
            return ( x >= min ) && ( x <= max );
        }

        inline
        bool
        box_contains_point( Bbox box, Point point )
        {
            return inside_bounds( point.x(), box.xmin(), box.xmax() )
                && inside_bounds( point.y(), box.ymin(), box.ymax() )
                && inside_bounds( point.z(), box.zmin(), box.zmax() );
        }

}; // RayBoxIntersection
