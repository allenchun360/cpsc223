#ifndef __CHOMP_H__
#define __CHOMP_H__

/**
 * Returns an array containing strings representing the possible states of Chomp played
 * on a pan of the given size.  The order of the strings in the array obeys the partial
 * order defined by reachability: if state X can be reached from state Y then X will
 * be in the array before Y.  The size of the resulting array is saved in the (simulated)
 * reference parameter.  It is the caller's responsibility to free the resulting array
 * and the strings therein.
 *
 * If there are memory allocation problems then the array or some of the strings my be NULL.
 *
 * @param rows a positive integer
 * @param cols an integer between 1 and 9 inclusive
 * @param n a pointer to a integer, non-NULL
 * @return an array of Chomp states topologically sorted starting with the final state
 */
char **chomp_states(size_t rows, size_t cols, size_t *n);

/**
 * Returns an array containing the states reachable in one move from the given state.
 * The array will be ordered by piece taken starting with the bottom left
 * piece and proceeding bottom to top within the leftmost column and then
 * repeating left to right for the other columns.
 * If there are memory allocation problems then the array or some of the strings my be NULL.
 * The size of the resulting array is saved in the (simulated) reference parameter.
 * It is the caller's responsibility to free the resulting array and the strings therein.
 *
 * @param s a chomp state, non-null
 * @param n a pointer to a integer, non-NULL
 * @return an array containing the successors of that state
 */
char **chomp_successors(const char *s, size_t *n);

#endif
