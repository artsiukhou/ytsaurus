//
// Copyright 2017 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// -----------------------------------------------------------------------------
// File: str_cat.h
// -----------------------------------------------------------------------------
//
// This package contains functions for efficiently concatenating and appending
// strings: `StrCat()` and `StrAppend()`. Most of the work within these routines
// is actually handled through use of a special AlphaNum type, which was
// designed to be used as a parameter type that efficiently manages conversion
// to strings and avoids copies in the above operations.
//
// Any routine accepting either a string or a number may accept `AlphaNum`.
// The basic idea is that by accepting a `const AlphaNum &` as an argument
// to your function, your callers will automagically convert bools, integers,
// and floating point values to strings for you.
//
// NOTE: Use of `AlphaNum` outside of the //y_absl/strings package is unsupported
// except for the specific case of function parameters of type `AlphaNum` or
// `const AlphaNum &`. In particular, instantiating `AlphaNum` directly as a
// stack variable is not supported.
//
// Conversion from 8-bit values is not accepted because, if it were, then an
// attempt to pass ':' instead of ":" might result in a 58 ending up in your
// result.
//
// Bools convert to "0" or "1". Pointers to types other than `char *` are not
// valid inputs. No output is generated for null `char *` pointers.
//
// Floating point numbers are formatted with six-digit precision, which is
// the default for "std::cout <<" or printf "%g" (the same as "%.6g").
//
// You can convert to hexadecimal output rather than decimal output using the
// `Hex` type contained here. To do so, pass `Hex(my_int)` as a parameter to
// `StrCat()` or `StrAppend()`. You may specify a minimum hex field width using
// a `PadSpec` enum.
//
// User-defined types can be formatted with the `AbslStringify()` customization
// point. The API relies on detecting an overload in the user-defined type's
// namespace of a free (non-member) `AbslStringify()` function as a definition
// (typically declared as a friend and implemented in-line.
// with the following signature:
//
// class MyClass { ... };
//
// template <typename Sink>
// void AbslStringify(Sink& sink, const MyClass& value);
//
// An `AbslStringify()` overload for a type should only be declared in the same
// file and namespace as said type.
//
// Note that `AbslStringify()` also supports use with `y_absl::StrFormat()` and
// `y_absl::Substitute()`.
//
// Example:
//
// struct Point {
//   // To add formatting support to `Point`, we simply need to add a free
//   // (non-member) function `AbslStringify()`. This method specifies how
//   // Point should be printed when y_absl::StrCat() is called on it. You can add
//   // such a free function using a friend declaration within the body of the
//   // class. The sink parameter is a templated type to avoid requiring
//   // dependencies.
//   template <typename Sink> friend void AbslStringify(Sink&
//   sink, const Point& p) {
//     y_absl::Format(&sink, "(%v, %v)", p.x, p.y);
//   }
//
//   int x;
//   int y;
// };
// -----------------------------------------------------------------------------

#ifndef Y_ABSL_STRINGS_STR_CAT_H_
#define Y_ABSL_STRINGS_STR_CAT_H_

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <util/generic/string.h>
#include <type_traits>
#include <utility>
#include <vector>

#include "y_absl/base/attributes.h"
#include "y_absl/base/nullability.h"
#include "y_absl/base/port.h"
#include "y_absl/meta/type_traits.h"
#include "y_absl/strings/has_absl_stringify.h"
#include "y_absl/strings/internal/resize_uninitialized.h"
#include "y_absl/strings/internal/stringify_sink.h"
#include "y_absl/strings/numbers.h"
#include "y_absl/strings/string_view.h"

namespace y_absl {
Y_ABSL_NAMESPACE_BEGIN

namespace strings_internal {
// AlphaNumBuffer allows a way to pass a string to StrCat without having to do
// memory allocation.  It is simply a pair of a fixed-size character array, and
// a size.  Please don't use outside of y_absl, yet.
template <size_t max_size>
struct AlphaNumBuffer {
  std::array<char, max_size> data;
  size_t size;
};

}  // namespace strings_internal

// Enum that specifies the number of significant digits to return in a `Hex` or
// `Dec` conversion and fill character to use. A `kZeroPad2` value, for example,
// would produce hexadecimal strings such as "0a","0f" and a 'kSpacePad5' value
// would produce hexadecimal strings such as "    a","    f".
enum PadSpec : uint8_t {
  kNoPad = 1,
  kZeroPad2,
  kZeroPad3,
  kZeroPad4,
  kZeroPad5,
  kZeroPad6,
  kZeroPad7,
  kZeroPad8,
  kZeroPad9,
  kZeroPad10,
  kZeroPad11,
  kZeroPad12,
  kZeroPad13,
  kZeroPad14,
  kZeroPad15,
  kZeroPad16,
  kZeroPad17,
  kZeroPad18,
  kZeroPad19,
  kZeroPad20,

  kSpacePad2 = kZeroPad2 + 64,
  kSpacePad3,
  kSpacePad4,
  kSpacePad5,
  kSpacePad6,
  kSpacePad7,
  kSpacePad8,
  kSpacePad9,
  kSpacePad10,
  kSpacePad11,
  kSpacePad12,
  kSpacePad13,
  kSpacePad14,
  kSpacePad15,
  kSpacePad16,
  kSpacePad17,
  kSpacePad18,
  kSpacePad19,
  kSpacePad20,
};

// -----------------------------------------------------------------------------
// Hex
// -----------------------------------------------------------------------------
//
// `Hex` stores a set of hexadecimal string conversion parameters for use
// within `AlphaNum` string conversions.
struct Hex {
  uint64_t value;
  uint8_t width;
  char fill;

  template <typename Int>
  explicit Hex(
      Int v, PadSpec spec = y_absl::kNoPad,
      typename std::enable_if<sizeof(Int) == 1 &&
                              !std::is_pointer<Int>::value>::type* = nullptr)
      : Hex(spec, static_cast<uint8_t>(v)) {}
  template <typename Int>
  explicit Hex(
      Int v, PadSpec spec = y_absl::kNoPad,
      typename std::enable_if<sizeof(Int) == 2 &&
                              !std::is_pointer<Int>::value>::type* = nullptr)
      : Hex(spec, static_cast<uint16_t>(v)) {}
  template <typename Int>
  explicit Hex(
      Int v, PadSpec spec = y_absl::kNoPad,
      typename std::enable_if<sizeof(Int) == 4 &&
                              !std::is_pointer<Int>::value>::type* = nullptr)
      : Hex(spec, static_cast<uint32_t>(v)) {}
  template <typename Int>
  explicit Hex(
      Int v, PadSpec spec = y_absl::kNoPad,
      typename std::enable_if<sizeof(Int) == 8 &&
                              !std::is_pointer<Int>::value>::type* = nullptr)
      : Hex(spec, static_cast<uint64_t>(v)) {}
  template <typename Pointee>
  explicit Hex(y_absl::Nullable<Pointee*> v, PadSpec spec = y_absl::kNoPad)
      : Hex(spec, reinterpret_cast<uintptr_t>(v)) {}

  template <typename S>
  friend void AbslStringify(S& sink, Hex hex) {
    static_assert(
        numbers_internal::kFastToBufferSize >= 32,
        "This function only works when output buffer >= 32 bytes long");
    char buffer[numbers_internal::kFastToBufferSize];
    char* const end = &buffer[numbers_internal::kFastToBufferSize];
    auto real_width =
        y_absl::numbers_internal::FastHexToBufferZeroPad16(hex.value, end - 16);
    if (real_width >= hex.width) {
      sink.Append(y_absl::string_view(end - real_width, real_width));
    } else {
      // Pad first 16 chars because FastHexToBufferZeroPad16 pads only to 16 and
      // max pad width can be up to 20.
      std::memset(end - 32, hex.fill, 16);
      // Patch up everything else up to the real_width.
      std::memset(end - real_width - 16, hex.fill, 16);
      sink.Append(y_absl::string_view(end - hex.width, hex.width));
    }
  }

 private:
  Hex(PadSpec spec, uint64_t v)
      : value(v),
        width(spec == y_absl::kNoPad
                  ? 1
                  : spec >= y_absl::kSpacePad2 ? spec - y_absl::kSpacePad2 + 2
                                             : spec - y_absl::kZeroPad2 + 2),
        fill(spec >= y_absl::kSpacePad2 ? ' ' : '0') {}
};

// -----------------------------------------------------------------------------
// Dec
// -----------------------------------------------------------------------------
//
// `Dec` stores a set of decimal string conversion parameters for use
// within `AlphaNum` string conversions.  Dec is slower than the default
// integer conversion, so use it only if you need padding.
struct Dec {
  uint64_t value;
  uint8_t width;
  char fill;
  bool neg;

  template <typename Int>
  explicit Dec(Int v, PadSpec spec = y_absl::kNoPad,
               typename std::enable_if<(sizeof(Int) <= 8)>::type* = nullptr)
      : value(v >= 0 ? static_cast<uint64_t>(v)
                     : uint64_t{0} - static_cast<uint64_t>(v)),
        width(spec == y_absl::kNoPad       ? 1
              : spec >= y_absl::kSpacePad2 ? spec - y_absl::kSpacePad2 + 2
                                         : spec - y_absl::kZeroPad2 + 2),
        fill(spec >= y_absl::kSpacePad2 ? ' ' : '0'),
        neg(v < 0) {}

  template <typename S>
  friend void AbslStringify(S& sink, Dec dec) {
    assert(dec.width <= numbers_internal::kFastToBufferSize);
    char buffer[numbers_internal::kFastToBufferSize];
    char* const end = &buffer[numbers_internal::kFastToBufferSize];
    char* const minfill = end - dec.width;
    char* writer = end;
    uint64_t val = dec.value;
    while (val > 9) {
      *--writer = '0' + (val % 10);
      val /= 10;
    }
    *--writer = '0' + static_cast<char>(val);
    if (dec.neg) *--writer = '-';

    ptrdiff_t fillers = writer - minfill;
    if (fillers > 0) {
      // Tricky: if the fill character is ' ', then it's <fill><+/-><digits>
      // But...: if the fill character is '0', then it's <+/-><fill><digits>
      bool add_sign_again = false;
      if (dec.neg && dec.fill == '0') {  // If filling with '0',
        ++writer;                    // ignore the sign we just added
        add_sign_again = true;       // and re-add the sign later.
      }
      writer -= fillers;
      std::fill_n(writer, fillers, dec.fill);
      if (add_sign_again) *--writer = '-';
    }

    sink.Append(y_absl::string_view(writer, static_cast<size_t>(end - writer)));
  }
};

// -----------------------------------------------------------------------------
// AlphaNum
// -----------------------------------------------------------------------------
//
// The `AlphaNum` class acts as the main parameter type for `StrCat()` and
// `StrAppend()`, providing efficient conversion of numeric, boolean, decimal,
// and hexadecimal values (through the `Dec` and `Hex` types) into strings.
// `AlphaNum` should only be used as a function parameter. Do not instantiate
//  `AlphaNum` directly as a stack variable.

class AlphaNum {
 public:
  // No bool ctor -- bools convert to an integral type.
  // A bool ctor would also convert incoming pointers (bletch).

  AlphaNum(int x)  // NOLINT(runtime/explicit)
      : piece_(digits_, static_cast<size_t>(
                            numbers_internal::FastIntToBuffer(x, digits_) -
                            &digits_[0])) {}
  AlphaNum(unsigned int x)  // NOLINT(runtime/explicit)
      : piece_(digits_, static_cast<size_t>(
                            numbers_internal::FastIntToBuffer(x, digits_) -
                            &digits_[0])) {}
  AlphaNum(long x)  // NOLINT(*)
      : piece_(digits_, static_cast<size_t>(
                            numbers_internal::FastIntToBuffer(x, digits_) -
                            &digits_[0])) {}
  AlphaNum(unsigned long x)  // NOLINT(*)
      : piece_(digits_, static_cast<size_t>(
                            numbers_internal::FastIntToBuffer(x, digits_) -
                            &digits_[0])) {}
  AlphaNum(long long x)  // NOLINT(*)
      : piece_(digits_, static_cast<size_t>(
                            numbers_internal::FastIntToBuffer(x, digits_) -
                            &digits_[0])) {}
  AlphaNum(unsigned long long x)  // NOLINT(*)
      : piece_(digits_, static_cast<size_t>(
                            numbers_internal::FastIntToBuffer(x, digits_) -
                            &digits_[0])) {}

  AlphaNum(float f)  // NOLINT(runtime/explicit)
      : piece_(digits_, numbers_internal::SixDigitsToBuffer(f, digits_)) {}
  AlphaNum(double f)  // NOLINT(runtime/explicit)
      : piece_(digits_, numbers_internal::SixDigitsToBuffer(f, digits_)) {}

  template <size_t size>
  AlphaNum(  // NOLINT(runtime/explicit)
      const strings_internal::AlphaNumBuffer<size>& buf
          Y_ABSL_ATTRIBUTE_LIFETIME_BOUND)
      : piece_(&buf.data[0], buf.size) {}

  AlphaNum(y_absl::Nullable<const char*> c_str  // NOLINT(runtime/explicit)
               Y_ABSL_ATTRIBUTE_LIFETIME_BOUND)
      : piece_(NullSafeStringView(c_str)) {}
  AlphaNum(y_absl::string_view pc  // NOLINT(runtime/explicit)
               Y_ABSL_ATTRIBUTE_LIFETIME_BOUND)
      : piece_(pc) {}

  template <typename T, typename = typename std::enable_if<
                            HasAbslStringify<T>::value>::type>
  AlphaNum(  // NOLINT(runtime/explicit)
      const T& v Y_ABSL_ATTRIBUTE_LIFETIME_BOUND,
      strings_internal::StringifySink&& sink Y_ABSL_ATTRIBUTE_LIFETIME_BOUND = {})
      : piece_(strings_internal::ExtractStringification(sink, v)) {}

  template <typename Allocator>
  AlphaNum(  // NOLINT(runtime/explicit)
      const std::basic_string<char, std::char_traits<char>, Allocator>& str
          Y_ABSL_ATTRIBUTE_LIFETIME_BOUND)
      : piece_(str) {}

  AlphaNum(const TString& str)
      : piece_(str.data(), str.size()) {}

  // Use string literals ":" instead of character literals ':'.
  AlphaNum(char c) = delete;  // NOLINT(runtime/explicit)

  AlphaNum(const AlphaNum&) = delete;
  AlphaNum& operator=(const AlphaNum&) = delete;

  y_absl::string_view::size_type size() const { return piece_.size(); }
  y_absl::Nullable<const char*> data() const { return piece_.data(); }
  y_absl::string_view Piece() const { return piece_; }

  // Match unscoped enums.  Use integral promotion so that a `char`-backed
  // enum becomes a wider integral type AlphaNum will accept.
  template <typename T,
            typename = typename std::enable_if<
                std::is_enum<T>{} && std::is_convertible<T, int>{} &&
                !HasAbslStringify<T>::value>::type>
  AlphaNum(T e)  // NOLINT(runtime/explicit)
      : AlphaNum(+e) {}

  // This overload matches scoped enums.  We must explicitly cast to the
  // underlying type, but use integral promotion for the same reason as above.
  template <typename T,
            typename std::enable_if<std::is_enum<T>{} &&
                                        !std::is_convertible<T, int>{} &&
                                        !HasAbslStringify<T>::value,
                                    char*>::type = nullptr>
  AlphaNum(T e)  // NOLINT(runtime/explicit)
      : AlphaNum(+static_cast<typename std::underlying_type<T>::type>(e)) {}

  // vector<bool>::reference and const_reference require special help to
  // convert to `AlphaNum` because it requires two user defined conversions.
  template <
      typename T,
      typename std::enable_if<
          std::is_class<T>::value &&
          (std::is_same<T, std::vector<bool>::reference>::value ||
           std::is_same<T, std::vector<bool>::const_reference>::value)>::type* =
          nullptr>
  AlphaNum(T e) : AlphaNum(static_cast<bool>(e)) {}  // NOLINT(runtime/explicit)

 private:
  y_absl::string_view piece_;
  char digits_[numbers_internal::kFastToBufferSize];
};

// -----------------------------------------------------------------------------
// StrCat()
// -----------------------------------------------------------------------------
//
// Merges given strings or numbers, using no delimiter(s), returning the merged
// result as a string.
//
// `StrCat()` is designed to be the fastest possible way to construct a string
// out of a mix of raw C strings, string_views, strings, bool values,
// and numeric values.
//
// Don't use `StrCat()` for user-visible strings. The localization process
// works poorly on strings built up out of fragments.
//
// For clarity and performance, don't use `StrCat()` when appending to a
// string. Use `StrAppend()` instead. In particular, avoid using any of these
// (anti-)patterns:
//
//   str.append(StrCat(...))
//   str += StrCat(...)
//   str = StrCat(str, ...)
//
// The last case is the worst, with a potential to change a loop
// from a linear time operation with O(1) dynamic allocations into a
// quadratic time operation with O(n) dynamic allocations.
//
// See `StrAppend()` below for more information.

namespace strings_internal {

// Do not call directly - this is not part of the public API.
TString CatPieces(std::initializer_list<y_absl::string_view> pieces);
void AppendPieces(y_absl::Nonnull<TString*> dest,
                  std::initializer_list<y_absl::string_view> pieces);

void STLStringAppendUninitializedAmortized(TString* dest, size_t to_append);

// `SingleArgStrCat` overloads take built-in `int`, `long` and `long long` types
// (signed / unsigned) to avoid ambiguity on the call side. If we used int32_t
// and int64_t, then at least one of the three (`int` / `long` / `long long`)
// would have been ambiguous when passed to `SingleArgStrCat`.
TString SingleArgStrCat(int x);
TString SingleArgStrCat(unsigned int x);
TString SingleArgStrCat(long x);                // NOLINT
TString SingleArgStrCat(unsigned long x);       // NOLINT
TString SingleArgStrCat(long long x);           // NOLINT
TString SingleArgStrCat(unsigned long long x);  // NOLINT
TString SingleArgStrCat(float x);
TString SingleArgStrCat(double x);

// `SingleArgStrAppend` overloads are defined here for the same reasons as with
// `SingleArgStrCat` above.
void SingleArgStrAppend(TString& str, int x);
void SingleArgStrAppend(TString& str, unsigned int x);
void SingleArgStrAppend(TString& str, long x);                // NOLINT
void SingleArgStrAppend(TString& str, unsigned long x);       // NOLINT
void SingleArgStrAppend(TString& str, long long x);           // NOLINT
void SingleArgStrAppend(TString& str, unsigned long long x);  // NOLINT

template <typename T,
          typename = std::enable_if_t<std::is_arithmetic<T>::value &&
                                      !std::is_same<T, char>::value &&
                                      !std::is_same<T, bool>::value>>
using EnableIfFastCase = T;

}  // namespace strings_internal

Y_ABSL_MUST_USE_RESULT inline TString StrCat() { return TString(); }

template <typename T>
Y_ABSL_MUST_USE_RESULT inline TString StrCat(
    strings_internal::EnableIfFastCase<T> a) {
  return strings_internal::SingleArgStrCat(a);
}
Y_ABSL_MUST_USE_RESULT inline TString StrCat(const AlphaNum& a) {
  return TString(a.data(), a.size());
}

Y_ABSL_MUST_USE_RESULT TString StrCat(const AlphaNum& a, const AlphaNum& b);
Y_ABSL_MUST_USE_RESULT TString StrCat(const AlphaNum& a, const AlphaNum& b,
                                        const AlphaNum& c);
Y_ABSL_MUST_USE_RESULT TString StrCat(const AlphaNum& a, const AlphaNum& b,
                                        const AlphaNum& c, const AlphaNum& d);

// Support 5 or more arguments
template <typename... AV>
Y_ABSL_MUST_USE_RESULT inline TString StrCat(
    const AlphaNum& a, const AlphaNum& b, const AlphaNum& c, const AlphaNum& d,
    const AlphaNum& e, const AV&... args) {
  return strings_internal::CatPieces(
      {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
       static_cast<const AlphaNum&>(args).Piece()...});
}

// -----------------------------------------------------------------------------
// StrAppend()
// -----------------------------------------------------------------------------
//
// Appends a string or set of strings to an existing string, in a similar
// fashion to `StrCat()`.
//
// WARNING: `StrAppend(&str, a, b, c, ...)` requires that none of the
// a, b, c, parameters be a reference into str. For speed, `StrAppend()` does
// not try to check each of its input arguments to be sure that they are not
// a subset of the string being appended to. That is, while this will work:
//
//   TString s = "foo";
//   s += s;
//
// This output is undefined:
//
//   TString s = "foo";
//   StrAppend(&s, s);
//
// This output is undefined as well, since `y_absl::string_view` does not own its
// data:
//
//   TString s = "foobar";
//   y_absl::string_view p = s;
//   StrAppend(&s, p);

inline void StrAppend(y_absl::Nonnull<TString*>) {}
void StrAppend(y_absl::Nonnull<TString*> dest, const AlphaNum& a);
void StrAppend(y_absl::Nonnull<TString*> dest, const AlphaNum& a,
               const AlphaNum& b);
void StrAppend(y_absl::Nonnull<TString*> dest, const AlphaNum& a,
               const AlphaNum& b, const AlphaNum& c);
void StrAppend(y_absl::Nonnull<TString*> dest, const AlphaNum& a,
               const AlphaNum& b, const AlphaNum& c, const AlphaNum& d);

// Support 5 or more arguments
template <typename... AV>
inline void StrAppend(y_absl::Nonnull<TString*> dest, const AlphaNum& a,
                      const AlphaNum& b, const AlphaNum& c, const AlphaNum& d,
                      const AlphaNum& e, const AV&... args) {
  strings_internal::AppendPieces(
      dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
             static_cast<const AlphaNum&>(args).Piece()...});
}

template <class String, class T>
std::enable_if_t<
    std::is_integral<y_absl::strings_internal::EnableIfFastCase<T>>::value, void>
StrAppend(y_absl::Nonnull<String*> result, T i) {
  return y_absl::strings_internal::SingleArgStrAppend(*result, i);
}

// This overload is only selected if all the parameters are numbers that can be
// handled quickly.
// Later we can look into how we can extend this to more general argument
// mixtures without bloating codegen too much, or copying unnecessarily.
#ifndef __NVCC__
template <typename String, typename... T>
std::enable_if_t<
    (sizeof...(T) > 1),
    std::common_type_t<std::conditional_t<
        true, void, y_absl::strings_internal::EnableIfFastCase<T>>...>>
StrAppend(y_absl::Nonnull<String*> str, T... args) {
  // Do not add unnecessary variables, logic, or even "free" lambdas here.
  // They can add overhead for the compiler and/or at run time.
  // Furthermore, assume this function will be inlined.
  // This function is carefully tailored to be able to be largely optimized away
  // so that it becomes near-equivalent to the caller handling each argument
  // individually while minimizing register pressure, so that the compiler
  // can inline it with minimal overhead.

  // First, calculate the total length, so we can perform just a single resize.
  // Save all the lengths for later.
  size_t total_length = 0;
  const ptrdiff_t lengths[] = {
      y_absl::numbers_internal::GetNumDigitsOrNegativeIfNegative(args)...};
  for (const ptrdiff_t possibly_negative_length : lengths) {
    // Lengths are negative for negative numbers. Keep them for later use, but
    // take their absolute values for calculating total lengths;
    total_length += possibly_negative_length < 0
                        ? static_cast<size_t>(-possibly_negative_length)
                        : static_cast<size_t>(possibly_negative_length);
  }

  // Now reserve space for all the arguments.
  const size_t old_size = str->size();
  y_absl::strings_internal::STLStringAppendUninitializedAmortized(str,
                                                                total_length);

  // Finally, output each argument one-by-one, from left to right.
  size_t i = 0;  // The current argument we're processing
  ptrdiff_t n;   // The length of the current argument
  typename String::pointer pos = &(*str)[old_size];
  using SomeTrivialEmptyType = std::false_type;
  // Ugly code due to the lack of C++14 fold expression makes us.
  const SomeTrivialEmptyType dummy1;
  for (const SomeTrivialEmptyType& dummy2 :
       {(/* Comma expressions are poor man's C++17 fold expression for C++14 */
         (void)(n = lengths[i]),
         (void)(n < 0 ? (void)(*pos++ = '-'), (n = ~n) : 0),
         (void)y_absl::numbers_internal::FastIntToBufferBackward(
             y_absl::numbers_internal::UnsignedAbsoluteValue(std::move(args)),
             pos += n, static_cast<uint32_t>(n)),
         (void)++i, dummy1)...}) {
    (void)dummy2;  // Remove & migrate to fold expressions in C++17
  }
}
#endif
// Helper function for the future StrCat default floating-point format, %.6g
// This is fast.
inline strings_internal::AlphaNumBuffer<
    numbers_internal::kSixDigitsToBufferSize>
SixDigits(double d) {
  strings_internal::AlphaNumBuffer<numbers_internal::kSixDigitsToBufferSize>
      result;
  result.size = numbers_internal::SixDigitsToBuffer(d, &result.data[0]);
  return result;
}

Y_ABSL_NAMESPACE_END
}  // namespace y_absl

#endif  // Y_ABSL_STRINGS_STR_CAT_H_
