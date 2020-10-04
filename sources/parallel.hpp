#pragma once

#include <execution>

namespace BigWord {
#if BIGWORD_USE_PARALLEL_STD
constexpr auto ExecutionPolicySequential = std::execution::seq;
constexpr auto ExecutionPolicyParallel = std::execution::par;
constexpr auto ExecutionPolicyParallelUnsequenced = std::execution::par_unseq;
#else
constexpr auto ExecutionPolicySequential = std::execution::seq;
constexpr auto ExecutionPolicyParallel = std::execution::seq;
constexpr auto ExecutionPolicyParallelUnsequenced = std::execution::seq;
#endif
}  // namespace BigWord
