///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "ices_types.hpp"

namespace ices {
    // Solve the iceberg avoiding problem for the given grid, using an exhaustive
    // optimization algorithm.
    //
    // This algorithm is expected to run in exponential time, so the grid's
    // width+height must be small enough to fit in a 64-bit int; this is enforced
    // with an assertion.
    //
    // The grid must be non-empty.
    unsigned int iceberg_avoiding_exhaustive(const grid& setting) {

        // grid must be non-empty.
        assert(setting.rows() > 0);
        assert(setting.columns() > 0);

        // Compute the path length, and check that it is legal.
        const size_t steps = setting.rows() + setting.columns() - 2;
        assert(steps < 64);

        unsigned int count_paths = 0;

        // Bit string will determine which direction to go.
        // If bit is 1, go right. If bit is 0, go down.
        // Increment count_paths by 1 only if the candidate is within the grid
        // and it never touches an iceberg.

        // Start at 0 and go all the way to (2**steps)-1.
        for (size_t bits = 0; bits <= pow(2, steps) - 1; bits++)
        {
            std::vector<step_direction> steps_after_begin; //Declare a new vector as a path.
            for (int k = 0; k <= steps - 1; k++)
            {
                int bit = (bits >> k) & 1;
                if (bit == 1)
                {
                    steps_after_begin.emplace_back(step_direction(STEP_DIRECTION_RIGHT));
                }
                else if (bit == 0)
                {
                    steps_after_begin.emplace_back(step_direction(STEP_DIRECTION_DOWN));
                }
            }
            path candidate(setting); //Declare a candidate with the setting.

            // Add the steps into the candidate.
            for (auto& step : steps_after_begin)
            {
                if (candidate.is_step_valid(step))
                {
                    candidate.add_step(step);
                }
            }

            // If the candidate reaches the bottom right corner, increment count_paths by 1.
            if ((candidate.final_row() == setting.rows() - 1) && (candidate.final_column() == setting.columns() - 1))
            {
                count_paths++;
            }
        }

        return count_paths;
    }

    // Solve the iceberg avoiding problem for the given grid, using a dynamic
    // programming algorithm.
    //
    // The grid must be non-empty.
    unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

        // grid must be non-empty.
        assert(setting.rows() > 0);
        assert(setting.columns() > 0);


        const int DIM = 100;
        std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));

        A[0][0] = 1;

        std::cout << std::endl;
        setting.print();
        std::cout << std::endl;

        for (int i = 0; i <= setting.rows() - 1; i++)
        {
            for (int j = 0; j <= setting.columns() - 1; j++)
            {
                //Exclude the starting point and continue.
                if ((i == 0) && (j == 0))
                {
                    continue;
                }
                //If there's an iceberg, set A[i][j] to 0 and continue.
                if (setting.get(i, j) == CELL_ICEBERG)
                {
                    A[i][j] = 0;
                    continue;
                }
                //Declare two int variables called right_moves and down_moves, and set both to 0.
                int right_moves = 0;
                int down_moves = 0;
                // As long as i and j are greater than 0 and neither A[i-1][j] nor A[i][j-1]
                // are 0, set right_moves and down_moves to their corresponding values.
                if ((i > 0) && (A[i - 1][j] != 0))
                {
                    right_moves = A[i - 1][j];
                }
                if ((j > 0) && (A[i][j - 1] != 0))
                {
                    down_moves = A[i][j - 1];
                }

                A[i][j] = A[i][j] + down_moves + right_moves; // Save the current value of A[i][j].
            }
        }

        return A[setting.rows() - 1][setting.columns() - 1];
    }

}