#ifndef __LOCATION_H__
#define __LOCATION_H__

typedef struct _location
{
  double lat;
  double lon;
} location;

/**
 * Determines if the given location is valid.  A location is valid if the
 * latitude is between -90 and 90 degrees.
 *
 * @param l a pointer to a location, non-NULL
 * @return a non-zero value if the location is valid, zero otherwise
 */
int location_validate(const location *l);

/**
 * Returns the distance between the two locations on the Earth's surface,
 * assuming a spherical earth with radius 6378.1 km.
 *
 * @param l1 a location
 * @param l2 a location
 * @return the distance between those points
 */
double location_distance(const location *l1, const location *l2);

#endif
