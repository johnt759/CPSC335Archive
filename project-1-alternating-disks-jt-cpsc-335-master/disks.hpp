///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the 
// alternating disks problem.
//
// As provided, this header has four functions marked with 
// TODO comments.
// You need to write in your own implementation of these 
// functions.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
    std::vector<disk_color> _colors;

public:

    disk_state(size_t light_count)
        : _colors(light_count * 2, DISK_LIGHT) {

        assert(dark_count() > 0);

        for (size_t i = 1; i < _colors.size(); i += 2) {
            _colors[i] = DISK_DARK;
        }
    }

    // Equality operator for unit tests.
    bool operator== (const disk_state& rhs) const {
        return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
    }

    size_t total_count() const {
        return _colors.size();
    }

    size_t dark_count() const {
        return total_count() / 2;
    }

    size_t light_count() const {
        return dark_count();
    }

    bool is_index(size_t i) const {
        return (i < total_count());
    }

    disk_color get(size_t index) const {
        assert(is_index(index));
        return _colors[index];
    }

    void swap(size_t left_index) {
        assert(is_index(left_index));
        auto right_index = left_index + 1;
        assert(is_index(right_index));
        std::swap(_colors[left_index], _colors[right_index]);
    }

    std::string to_string() const {
        std::stringstream ss;
        bool first = true;
        for (auto color : _colors) {
            if (!first) {
                ss << " ";
            }

            if (color == DISK_LIGHT) {
                ss << "L";
            }
            else {
                ss << "D";
            }

            first = false;
        }
        return ss.str();
    }

    // Return true when this disk_state is in alternating format. That means
    // that the first disk at index 0 is light, the second disk at index 1
    // is dark, and so on for the entire row of disks.
    bool is_alternating() const {
        bool isAlternating = true;
        int arraySize = total_count();

        for (int i = 0; i < arraySize; i += 2)
        {
            // Return false if the current disk is not light and next disk is not dark.
            if ((get(i) != DISK_LIGHT) || (get(i + 1) != DISK_DARK))
            {
                isAlternating = false;
                break;
            }
        }

        return isAlternating;
    }

    // Return true when this disk_state is fully sorted, with all light disks
    // on the left (low indices) and all dark disks on the right (high
    // indices).
    bool is_sorted() const {
        bool isSorted = true;
        int arraySizeHalf = total_count() / 2;

        for (int i = 0; i < arraySizeHalf; i++)
        {
            // Return false if all of left disks are not light and all of right disks are not dark.
            if ((get(i) != DISK_LIGHT) || (get(i + arraySizeHalf) != DISK_DARK))
            {
                isSorted = false;
                break;
            }
        }

        return isSorted;
    }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
    disk_state _after;
    unsigned _swap_count;

public:

    sorted_disks(const disk_state& after, unsigned swap_count)
        : _after(after), _swap_count(swap_count) { }

    sorted_disks(disk_state&& after, unsigned swap_count)
        : _after(after), _swap_count(swap_count) { }

    const disk_state& after() const {
        return _after;
    }

    unsigned swap_count() const {
        return _swap_count;
    }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
    // check that the input is in alternating format  
    assert(before.is_alternating());
    auto after(before); // Have the auto variable inherit the disk_state variable before.
    int totalSwaps = 0;
    int numSwap = 0;

    // The left-to-right sorting will perform n times.
    for (int n = 0; n < after.total_count(); n++)
    {
        numSwap = 0;
        for (size_t i = 0; i < after.total_count() - 1; i++)
        {
            if (after.get(i) == DISK_DARK && after.get(i + 1) == DISK_LIGHT)
            {
                after.swap(i);
                numSwap++;
            }
        }
        totalSwaps += numSwap;
    }
    return sorted_disks(after, totalSwaps);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
    // check that the input is in alternating format
    assert(before.is_alternating());
    auto after(before); // Have the auto variable inherit the disk_state variable before.
    int totalSwaps = 0;
    int numSwap = 0;

    // Follow the same procedure for sort_left_to_right, except after reaching the
    // rightmost disk, perform the same operation in reverse up to leftmost disk.
    // The lawnmower sorting will perform n/2 times.
    for (int n = 0; n < after.total_count()/2; n++)
    {
        numSwap = 0;
        for (size_t i = 0; i < after.total_count() - 1; i++)
        {
            if (after.get(i) == DISK_DARK && after.get(i + 1) == DISK_LIGHT)
            {
                after.swap(i);
                numSwap++;
            }
        }
        totalSwaps += numSwap;

        numSwap = 0;
        for (size_t j = after.total_count() - 1; j >= 1; j--)
        {
            if (after.get(j) == DISK_LIGHT && after.get(j - 1) == DISK_DARK)
            {
                after.swap(j - 1);
                numSwap++;
            }
        }
        totalSwaps += numSwap;
    }
    return sorted_disks(after, totalSwaps);
}