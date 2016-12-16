// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Take the name prefix into account.
#define yylex RLSVGPath_lex

// First part of user declarations.

#ifndef YY_NULLPTR
#if defined __cplusplus && 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#endif

#include "SVGPathParser.h"

// User implementation prologue.

// Unqualified %code blocks.

#include "RLDecls.h"

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h>  // FIXME: INFRINGES ON USER NAME SPACE.
#define YY_(msgid) dgettext("bison-runtime", msgid)
#endif
#endif
#ifndef YY_
#define YY_(msgid) msgid
#endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                       \
  do                                                          \
    if (N) {                                                  \
      (Current).begin = YYRHSLOC(Rhs, 1).begin;               \
      (Current).end = YYRHSLOC(Rhs, N).end;                   \
    } else {                                                  \
      (Current).begin = (Current).end = YYRHSLOC(Rhs, 0).end; \
    }                                                         \
  while (/*CONSTCOND*/ false)
#endif

// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void)(E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
#define YYCDEBUG \
  if (yydebug_)  \
  (*yycdebug_)

#define YY_SYMBOL_PRINT(Title, Symbol) \
  do {                                 \
    if (yydebug_) {                    \
      *yycdebug_ << Title << ' ';      \
      yy_print_(*yycdebug_, Symbol);   \
      *yycdebug_ << std::endl;         \
    }                                  \
  } while (false)

#define YY_REDUCE_PRINT(Rule) \
  do {                        \
    if (yydebug_)             \
      yy_reduce_print_(Rule); \
  } while (false)

#define YY_STACK_PRINT() \
  do {                   \
    if (yydebug_)        \
      yystack_print_();  \
  } while (false)

#else  // !YYDEBUG

#define YYCDEBUG \
  if (false)     \
  std::cerr
#define YY_SYMBOL_PRINT(Title, Symbol) YYUSE(Symbol)
#define YY_REDUCE_PRINT(Rule) static_cast<void>(0)
#define YY_STACK_PRINT() static_cast<void>(0)

#endif  // !YYDEBUG

#define yyerrok (yyerrstatus_ = 0)
#define yyclearin (yyla.clear())

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab
#define YYRECOVERING() (!!yyerrstatus_)

namespace rl {

/* Return YYSTR after stripping away unnecessary quotes and
   backslashes, so that it's suitable for yyerror.  The heuristic is
   that double-quoting is unnecessary unless the string contains an
   apostrophe, a comma, or backslash (other than backslash-backslash).
   YYSTR is taken from yytname.  */
std::string SVGPathParser::yytnamerr_(const char* yystr) {
  if (*yystr == '"') {
    std::string yyr = "";
    char const* yyp = yystr;

    for (;;)
      switch (*++yyp) {
        case '\'':
        case ',':
          goto do_not_strip_quotes;

        case '\\':
          if (*++yyp != '\\')
            goto do_not_strip_quotes;
        // Fall through.
        default:
          yyr += *yyp;
          break;

        case '"':
          return yyr;
      }
  do_not_strip_quotes:;
  }

  return yystr;
}

/// Build a parser object.
SVGPathParser::SVGPathParser(rl::SVGPathDriver& driver_yyarg,
                             void* scanner_yyarg)
    :
#if YYDEBUG
      yydebug_(false),
      yycdebug_(&std::cerr),
#endif
      driver(driver_yyarg),
      scanner(scanner_yyarg) {
}

SVGPathParser::~SVGPathParser() {}

/*---------------.
| Symbol types.  |
`---------------*/

// by_state.
inline SVGPathParser::by_state::by_state() : state(empty_state) {}

inline SVGPathParser::by_state::by_state(const by_state& other)
    : state(other.state) {}

inline void SVGPathParser::by_state::clear() {
  state = empty_state;
}

inline void SVGPathParser::by_state::move(by_state& that) {
  state = that.state;
  that.clear();
}

inline SVGPathParser::by_state::by_state(state_type s) : state(s) {}

inline SVGPathParser::symbol_number_type SVGPathParser::by_state::type_get()
    const {
  if (state == empty_state)
    return empty_symbol;
  else
    return yystos_[state];
}

inline SVGPathParser::stack_symbol_type::stack_symbol_type() {}

inline SVGPathParser::stack_symbol_type::stack_symbol_type(state_type s,
                                                           symbol_type& that)
    : super_type(s, that.location) {
  switch (that.type_get()) {
    case 41:  // ArcParam
      value.move<rl::SVGArcParam>(that.value);
      break;

    case 28:  // Close
      value.move<rl::SVGCloseElement>(that.value);
      break;

    case 32:  // Curve
      value.move<rl::SVGCurveElement>(that.value);
      break;

    case 26:  // PathElement
      value.move<rl::SVGElement>(that.value);
      break;

    case 36:  // EllipticArc
      value.move<rl::SVGEllipticArcElement>(that.value);
      break;

    case 29:  // Line
      value.move<rl::SVGLineElement>(that.value);
      break;

    case 30:  // LineHorizontal
      value.move<rl::SVGLineHorizontalElement>(that.value);
      break;

    case 31:  // LineVertical
      value.move<rl::SVGLineVerticalElement>(that.value);
      break;

    case 27:  // Move
      value.move<rl::SVGMoveElement>(that.value);
      break;

    case 23:  // "<number>"
      value.move<rl::SVGNumber>(that.value);
      break;

    case 34:  // QuadCurve
      value.move<rl::SVGQuadCurveElement>(that.value);
      break;

    case 33:  // ShorthandCurve
      value.move<rl::SVGShorthandCurveElement>(that.value);
      break;

    case 35:  // ShorthandQuadCurve
      value.move<rl::SVGShorthandQuadCurveElement>(that.value);
      break;

    case 38:  // XYCoordinate
      value.move<rl::geom::Point>(that.value);
      break;

    case 40:  // ArcParams
      value.move<std::vector<rl::SVGArcParam>>(that.value);
      break;

    case 39:  // Numbers
      value.move<std::vector<rl::SVGNumber>>(that.value);
      break;

    case 37:  // XYCoordinates
      value.move<std::vector<rl::geom::Point>>(that.value);
      break;

    default:
      break;
  }

  // that is emptied.
  that.type = empty_symbol;
}

inline SVGPathParser::stack_symbol_type& SVGPathParser::stack_symbol_type::
operator=(const stack_symbol_type& that) {
  state = that.state;
  switch (that.type_get()) {
    case 41:  // ArcParam
      value.copy<rl::SVGArcParam>(that.value);
      break;

    case 28:  // Close
      value.copy<rl::SVGCloseElement>(that.value);
      break;

    case 32:  // Curve
      value.copy<rl::SVGCurveElement>(that.value);
      break;

    case 26:  // PathElement
      value.copy<rl::SVGElement>(that.value);
      break;

    case 36:  // EllipticArc
      value.copy<rl::SVGEllipticArcElement>(that.value);
      break;

    case 29:  // Line
      value.copy<rl::SVGLineElement>(that.value);
      break;

    case 30:  // LineHorizontal
      value.copy<rl::SVGLineHorizontalElement>(that.value);
      break;

    case 31:  // LineVertical
      value.copy<rl::SVGLineVerticalElement>(that.value);
      break;

    case 27:  // Move
      value.copy<rl::SVGMoveElement>(that.value);
      break;

    case 23:  // "<number>"
      value.copy<rl::SVGNumber>(that.value);
      break;

    case 34:  // QuadCurve
      value.copy<rl::SVGQuadCurveElement>(that.value);
      break;

    case 33:  // ShorthandCurve
      value.copy<rl::SVGShorthandCurveElement>(that.value);
      break;

    case 35:  // ShorthandQuadCurve
      value.copy<rl::SVGShorthandQuadCurveElement>(that.value);
      break;

    case 38:  // XYCoordinate
      value.copy<rl::geom::Point>(that.value);
      break;

    case 40:  // ArcParams
      value.copy<std::vector<rl::SVGArcParam>>(that.value);
      break;

    case 39:  // Numbers
      value.copy<std::vector<rl::SVGNumber>>(that.value);
      break;

    case 37:  // XYCoordinates
      value.copy<std::vector<rl::geom::Point>>(that.value);
      break;

    default:
      break;
  }

  location = that.location;
  return *this;
}

template <typename Base>
inline void SVGPathParser::yy_destroy_(const char* yymsg,
                                       basic_symbol<Base>& yysym) const {
  if (yymsg)
    YY_SYMBOL_PRINT(yymsg, yysym);
}

#if YYDEBUG
template <typename Base>
void SVGPathParser::yy_print_(std::ostream& yyo,
                              const basic_symbol<Base>& yysym) const {
  std::ostream& yyoutput = yyo;
  YYUSE(yyoutput);
  symbol_number_type yytype = yysym.type_get();
  // Avoid a (spurious) G++ 4.8 warning about "array subscript is
  // below array bounds".
  if (yysym.empty())
    std::abort();
  yyo << (yytype < yyntokens_ ? "token" : "nterm") << ' ' << yytname_[yytype]
      << " (" << yysym.location << ": ";
  YYUSE(yytype);
  yyo << ')';
}
#endif

inline void SVGPathParser::yypush_(const char* m,
                                   state_type s,
                                   symbol_type& sym) {
  stack_symbol_type t(s, sym);
  yypush_(m, t);
}

inline void SVGPathParser::yypush_(const char* m, stack_symbol_type& s) {
  if (m)
    YY_SYMBOL_PRINT(m, s);
  yystack_.push(s);
}

inline void SVGPathParser::yypop_(unsigned int n) {
  yystack_.pop(n);
}

#if YYDEBUG
std::ostream& SVGPathParser::debug_stream() const {
  return *yycdebug_;
}

void SVGPathParser::set_debug_stream(std::ostream& o) {
  yycdebug_ = &o;
}

SVGPathParser::debug_level_type SVGPathParser::debug_level() const {
  return yydebug_;
}

void SVGPathParser::set_debug_level(debug_level_type l) {
  yydebug_ = l;
}
#endif  // YYDEBUG

inline SVGPathParser::state_type SVGPathParser::yy_lr_goto_state_(
    state_type yystate,
    int yysym) {
  int yyr = yypgoto_[yysym - yyntokens_] + yystate;
  if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
    return yytable_[yyr];
  else
    return yydefgoto_[yysym - yyntokens_];
}

inline bool SVGPathParser::yy_pact_value_is_default_(int yyvalue) {
  return yyvalue == yypact_ninf_;
}

inline bool SVGPathParser::yy_table_value_is_error_(int yyvalue) {
  return yyvalue == yytable_ninf_;
}

int SVGPathParser::parse() {
  // State.
  int yyn;
  /// Length of the RHS of the rule being reduced.
  int yylen = 0;

  // Error handling.
  int yynerrs_ = 0;
  int yyerrstatus_ = 0;

  /// The lookahead symbol.
  symbol_type yyla;

  /// The locations where the error started and ended.
  stack_symbol_type yyerror_range[3];

  /// The return value of parse ().
  int yyresult;

  // FIXME: This shoud be completely indented.  It is not yet to
  // avoid gratuitous conflicts when merging into the master branch.
  try {
    YYCDEBUG << "Starting parse" << std::endl;

    // User initialization code.

    {}

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear();
    yypush_(YY_NULLPTR, 0, yyla);

  // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

  // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_(yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty()) {
      YYCDEBUG << "Reading a token: ";
      try {
        symbol_type yylookahead(yylex(driver, scanner));
        yyla.move(yylookahead);
      } catch (const syntax_error& yyexc) {
        error(yyexc);
        goto yyerrlab1;
      }
    }
    YY_SYMBOL_PRINT("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0) {
      if (yy_table_value_is_error_(yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn]) {
        case 41:  // ArcParam
          yylhs.value.build<rl::SVGArcParam>();
          break;

        case 28:  // Close
          yylhs.value.build<rl::SVGCloseElement>();
          break;

        case 32:  // Curve
          yylhs.value.build<rl::SVGCurveElement>();
          break;

        case 26:  // PathElement
          yylhs.value.build<rl::SVGElement>();
          break;

        case 36:  // EllipticArc
          yylhs.value.build<rl::SVGEllipticArcElement>();
          break;

        case 29:  // Line
          yylhs.value.build<rl::SVGLineElement>();
          break;

        case 30:  // LineHorizontal
          yylhs.value.build<rl::SVGLineHorizontalElement>();
          break;

        case 31:  // LineVertical
          yylhs.value.build<rl::SVGLineVerticalElement>();
          break;

        case 27:  // Move
          yylhs.value.build<rl::SVGMoveElement>();
          break;

        case 23:  // "<number>"
          yylhs.value.build<rl::SVGNumber>();
          break;

        case 34:  // QuadCurve
          yylhs.value.build<rl::SVGQuadCurveElement>();
          break;

        case 33:  // ShorthandCurve
          yylhs.value.build<rl::SVGShorthandCurveElement>();
          break;

        case 35:  // ShorthandQuadCurve
          yylhs.value.build<rl::SVGShorthandQuadCurveElement>();
          break;

        case 38:  // XYCoordinate
          yylhs.value.build<rl::geom::Point>();
          break;

        case 40:  // ArcParams
          yylhs.value.build<std::vector<rl::SVGArcParam>>();
          break;

        case 39:  // Numbers
          yylhs.value.build<std::vector<rl::SVGNumber>>();
          break;

        case 37:  // XYCoordinates
          yylhs.value.build<std::vector<rl::geom::Point>>();
          break;

        default:
          break;
      }

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice(yystack_, yylen);
        YYLLOC_DEFAULT(yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT(yyn);
      try {
        switch (yyn) {
          case 4:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGMoveElement>());
          }

          break;

          case 5:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGCloseElement>());
          }

          break;

          case 6:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGLineElement>());
          }

          break;

          case 7:

          {
            driver.visitElement(
                yystack_[0].value.as<rl::SVGLineHorizontalElement>());
          }

          break;

          case 8:

          {
            driver.visitElement(
                yystack_[0].value.as<rl::SVGLineVerticalElement>());
          }

          break;

          case 9:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGCurveElement>());
          }

          break;

          case 10:

          {
            driver.visitElement(
                yystack_[0].value.as<rl::SVGShorthandCurveElement>());
          }

          break;

          case 11:

          {
            driver.visitElement(
                yystack_[0].value.as<rl::SVGQuadCurveElement>());
          }

          break;

          case 12:

          {
            driver.visitElement(
                yystack_[0].value.as<rl::SVGShorthandQuadCurveElement>());
          }

          break;

          case 13:

          {
            driver.visitElement(
                yystack_[0].value.as<rl::SVGEllipticArcElement>());
          }

          break;

          case 14:

          {
            yylhs.value.as<rl::SVGMoveElement>() = rl::SVGMoveElement{
                true, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 15:

          {
            yylhs.value.as<rl::SVGMoveElement>() = rl::SVGMoveElement{
                false, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 16:

          {
            yylhs.value.as<rl::SVGCloseElement>() = rl::SVGCloseElement{};
          }

          break;

          case 17:

          {
            yylhs.value.as<rl::SVGLineElement>() = rl::SVGLineElement{
                true, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 18:

          {
            yylhs.value.as<rl::SVGLineElement>() = rl::SVGLineElement{
                false, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 19:

          {
            yylhs.value.as<rl::SVGLineHorizontalElement>() =
                rl::SVGLineHorizontalElement{
                    true, yystack_[0].value.as<std::vector<rl::SVGNumber>>()};
          }

          break;

          case 20:

          {
            yylhs.value.as<rl::SVGLineHorizontalElement>() =
                rl::SVGLineHorizontalElement{
                    false, yystack_[0].value.as<std::vector<rl::SVGNumber>>()};
          }

          break;

          case 21:

          {
            yylhs.value.as<rl::SVGLineVerticalElement>() =
                rl::SVGLineVerticalElement{
                    true, yystack_[0].value.as<std::vector<rl::SVGNumber>>()};
          }

          break;

          case 22:

          {
            yylhs.value.as<rl::SVGLineVerticalElement>() =
                rl::SVGLineVerticalElement{
                    false, yystack_[0].value.as<std::vector<rl::SVGNumber>>()};
          }

          break;

          case 23:

          {
            yylhs.value.as<rl::SVGCurveElement>() = rl::SVGCurveElement{
                true, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 24:

          {
            yylhs.value.as<rl::SVGCurveElement>() = rl::SVGCurveElement{
                false, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 25:

          {
            yylhs.value.as<rl::SVGShorthandCurveElement>() =
                rl::SVGShorthandCurveElement{
                    true, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 26:

          {
            yylhs.value.as<rl::SVGShorthandCurveElement>() =
                rl::SVGShorthandCurveElement{
                    false,
                    yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 27:

          {
            yylhs.value.as<rl::SVGQuadCurveElement>() = rl::SVGQuadCurveElement{
                true, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 28:

          {
            yylhs.value.as<rl::SVGQuadCurveElement>() = rl::SVGQuadCurveElement{
                false, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 29:

          {
            yylhs.value.as<rl::SVGShorthandQuadCurveElement>() =
                rl::SVGShorthandQuadCurveElement{
                    true, yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 30:

          {
            yylhs.value.as<rl::SVGShorthandQuadCurveElement>() =
                rl::SVGShorthandQuadCurveElement{
                    false,
                    yystack_[0].value.as<std::vector<rl::geom::Point>>()};
          }

          break;

          case 31:

          {
            yylhs.value.as<rl::SVGEllipticArcElement>() =
                rl::SVGEllipticArcElement{
                    true, yystack_[0].value.as<std::vector<rl::SVGArcParam>>()};
          }

          break;

          case 32:

          {
            yylhs.value.as<rl::SVGEllipticArcElement>() =
                rl::SVGEllipticArcElement{
                    false,
                    yystack_[0].value.as<std::vector<rl::SVGArcParam>>()};
          }

          break;

          case 33:

          {
            yylhs.value.as<std::vector<rl::geom::Point>>() = {
                yystack_[0].value.as<rl::geom::Point>()};
          }

          break;

          case 34:

          {
            yystack_[1].value.as<std::vector<rl::geom::Point>>().emplace_back(
                yystack_[0].value.as<rl::geom::Point>());
          }

          break;

          case 35:

          {
            yylhs.value.as<rl::geom::Point>() =
                rl::geom::Point(yystack_[1].value.as<rl::SVGNumber>(),
                                yystack_[0].value.as<rl::SVGNumber>());
          }

          break;

          case 36:

          {
            yylhs.value.as<std::vector<rl::SVGNumber>>() = {
                yystack_[0].value.as<rl::SVGNumber>()};
          }

          break;

          case 37:

          {
            yystack_[1].value.as<std::vector<rl::SVGNumber>>().emplace_back(
                yystack_[0].value.as<rl::SVGNumber>());
          }

          break;

          case 38:

          {
            yylhs.value.as<std::vector<rl::SVGArcParam>>() = {
                yystack_[0].value.as<rl::SVGArcParam>()};
          }

          break;

          case 39:

          {
            yystack_[1].value.as<std::vector<rl::SVGArcParam>>().emplace_back(
                yystack_[0].value.as<rl::SVGArcParam>());
          }

          break;

          case 40:

          {
            yylhs.value.as<rl::SVGArcParam>() =
                rl::SVGArcParam{yystack_[6].value.as<rl::SVGNumber>(),
                                yystack_[5].value.as<rl::SVGNumber>(),
                                yystack_[4].value.as<rl::SVGNumber>(),
                                yystack_[3].value.as<rl::SVGNumber>(),
                                yystack_[2].value.as<rl::SVGNumber>(),
                                yystack_[1].value.as<rl::SVGNumber>(),
                                yystack_[0].value.as<rl::SVGNumber>()};
          }

          break;

          default:
            break;
        }
      } catch (const syntax_error& yyexc) {
        error(yyexc);
        YYERROR;
      }
      YY_SYMBOL_PRINT("-> $$ =", yylhs);
      yypop_(yylen);
      yylen = 0;
      YY_STACK_PRINT();

      // Shift the result of the reduction.
      yypush_(YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_) {
      ++yynerrs_;
      error(yyla.location, yysyntax_error_(yystack_[0].state, yyla));
    }

    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3) {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      // Return failure if at end of input.
      if (yyla.type_get() == yyeof_)
        YYABORT;
      else if (!yyla.empty()) {
        yy_destroy_("Error: discarding", yyla);
        yyla.clear();
      }
    }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;

  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_(yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;  // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;) {
        yyn = yypact_[yystack_[0].state];
        if (!yy_pact_value_is_default_(yyn)) {
          yyn += yyterror_;
          if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_) {
            yyn = yytable_[yyn];
            if (0 < yyn)
              break;
          }
        }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size() == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_("Error: popping", yystack_[0]);
        yypop_();
        YY_STACK_PRINT();
      }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT(error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_("Shifting", error_token);
    }
    goto yynewstate;

  // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

  // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty())
      yy_destroy_("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_(yylen);
    while (1 < yystack_.size()) {
      yy_destroy_("Cleanup: popping", yystack_[0]);
      yypop_();
    }

    return yyresult;
  } catch (...) {
    YYCDEBUG << "Exception caught: cleaning lookahead and stack" << std::endl;
    // Do not try to display the values of the reclaimed symbols,
    // as their printer might throw an exception.
    if (!yyla.empty())
      yy_destroy_(YY_NULLPTR, yyla);

    while (1 < yystack_.size()) {
      yy_destroy_(YY_NULLPTR, yystack_[0]);
      yypop_();
    }
    throw;
  }
}

void SVGPathParser::error(const syntax_error& yyexc) {
  error(yyexc.location, yyexc.what());
}

// Generate an error message.
std::string SVGPathParser::yysyntax_error_(state_type yystate,
                                           const symbol_type& yyla) const {
  // Number of reported tokens (one for the "unexpected", one per
  // "expected").
  size_t yycount = 0;
  // Its maximum.
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  // Arguments of yyformat.
  char const* yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yyla) is
       if this state is a consistent state with a default action.
       Thus, detecting the absence of a lookahead is sufficient to
       determine that there is no unexpected or expected token to
       report.  In that case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is
       a consistent state with a default action.  There might have
       been a previous inconsistent state, consistent state with a
       non-default action, or user semantic action that manipulated
       yyla.  (However, yyla is currently not documented for users.)
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state
       merging (from LALR or IELR) and default reductions corrupt the
       expected token list.  However, the list is correct for
       canonical LR with one exception: it will still contain any
       token that will not be accepted due to an error action in a
       later state.
  */
  if (!yyla.empty()) {
    int yytoken = yyla.type_get();
    yyarg[yycount++] = yytname_[yytoken];
    int yyn = yypact_[yystate];
    if (!yy_pact_value_is_default_(yyn)) {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      // Stay within bounds of both yycheck and yytname.
      int yychecklim = yylast_ - yyn + 1;
      int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
      for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_ &&
            !yy_table_value_is_error_(yytable_[yyx + yyn])) {
          if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM) {
            yycount = 1;
            break;
          } else
            yyarg[yycount++] = yytname_[yyx];
        }
    }
  }

  char const* yyformat = YY_NULLPTR;
  switch (yycount) {
#define YYCASE_(N, S) \
  case N:             \
    yyformat = S;     \
    break
    YYCASE_(0, YY_("syntax error"));
    YYCASE_(1, YY_("syntax error, unexpected %s"));
    YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
    YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
    YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
    YYCASE_(5,
            YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
  }

  std::string yyres;
  // Argument number.
  size_t yyi = 0;
  for (char const* yyp = yyformat; *yyp; ++yyp)
    if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount) {
      yyres += yytnamerr_(yyarg[yyi++]);
      ++yyp;
    } else
      yyres += *yyp;
  return yyres;
}

const signed char SVGPathParser::yypact_ninf_ = -7;

const signed char SVGPathParser::yytable_ninf_ = -1;

const signed char SVGPathParser::yypact_[] = {
    19, 20, 20, -7, 20, 20, 25, 25, 25, 25, 20, 20, 20, 20, 20, 20, 20,
    20, 27, 27, 0,  -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, 28, 20,
    -7, 20, 20, 20, -7, 38, 38, 38, 38, 20, 20, 20, 20, 20, 20, 20, 20,
    41, 27, -7, 27, -7, -7, -7, -7, -7, 42, -7, 43, 44, 53, 54, -7};

const unsigned char SVGPathParser::yydefact_[] = {
    0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  2,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 0,  14,
    33, 15, 17, 18, 36, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    0,  31, 38, 32, 1,  3,  35, 34, 37, 0,  39, 0,  0,  0,  0,  40};

const signed char SVGPathParser::yypgoto_[] = {
    -7, -7, 59, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, 58, 9, -6, 61, -5};

const signed char SVGPathParser::yydefgoto_[] = {
    -1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 33, 34, 39, 52, 53};

const unsigned char SVGPathParser::yytable_[] = {
    55, 40, 41, 42, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
    12, 13, 14, 15, 16, 17, 18, 19, 58, 32, 58, 58, 58, 61, 38, 61, 51,
    57, 58, 58, 58, 58, 58, 58, 58, 58, 35, 59, 36, 37, 60, 62, 63, 64,
    43, 44, 45, 46, 47, 48, 49, 50, 65, 66, 0,  56, 54};

const signed char SVGPathParser::yycheck_[] = {
    0,  7,  8,  9,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 33, 23, 35, 36, 37, 52, 23, 54, 23,
    23, 43, 44, 45, 46, 47, 48, 49, 50, 2,  23, 4,  5,  23, 23, 23, 23,
    10, 11, 12, 13, 14, 15, 16, 17, 23, 23, -1, 20, 19};

const unsigned char SVGPathParser::yystos_[] = {
    0,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 23, 37,
    38, 37, 37, 37, 23, 39, 39, 39, 39, 37, 37, 37, 37, 37, 37, 37, 37,
    23, 40, 41, 40, 0,  26, 23, 38, 23, 23, 41, 23, 23, 23, 23, 23};

const unsigned char SVGPathParser::yyr1_[] = {
    0,  24, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    27, 27, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34,
    34, 35, 35, 36, 36, 37, 37, 38, 39, 39, 40, 40, 41};

const unsigned char SVGPathParser::yyr2_[] = {
    0, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 7};

// YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
// First, the terminals, then, starting at \a yyntokens_, nonterminals.
const char* const SVGPathParser::yytname_[] = {
    "\"<end of contents>\"",
    "error",
    "$undefined",
    "\"<invalid token>\"",
    "\"MoveTo (Absolute)\"",
    "\"MoveTo (Relative)\"",
    "\"Close\"",
    "\"Line (Absolute)\"",
    "\"Line (Relative)\"",
    "\"Line Horizontal (Absolute)\"",
    "\"Line Horizontal (Relative)\"",
    "\"Line Vertical (Absolute)\"",
    "\"Line Vertical (Relative)\"",
    "\"Curve (Absolute)\"",
    "\"Curve (Relative)\"",
    "\"Shorthand Curve (Absolute)\"",
    "\"Shorthand Curve (Relative)\"",
    "\"Quad Curve (Absolute)\"",
    "\"Quad Curve (Relative)\"",
    "\"Shorthand Quad Curve (Absolute)\"",
    "\"Shorthand Quad Curve (Relative)\"",
    "\"Elliptic Arc (Absolute)\"",
    "\"Elliptic Arc (Relative)\"",
    "\"<number>\"",
    "$accept",
    "PathElements",
    "PathElement",
    "Move",
    "Close",
    "Line",
    "LineHorizontal",
    "LineVertical",
    "Curve",
    "ShorthandCurve",
    "QuadCurve",
    "ShorthandQuadCurve",
    "EllipticArc",
    "XYCoordinates",
    "XYCoordinate",
    "Numbers",
    "ArcParams",
    "ArcParam",
    YY_NULLPTR};

#if YYDEBUG
const unsigned char SVGPathParser::yyrline_[] = {
    0,   85,  85,  86,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
    103, 104, 108, 112, 113, 117, 118, 122, 123, 127, 128, 132, 133, 137,
    138, 142, 143, 147, 148, 152, 153, 157, 161, 162, 166, 167, 171};

// Print the state stack on the debug stream.
void SVGPathParser::yystack_print_() {
  *yycdebug_ << "Stack now";
  for (stack_type::const_iterator i = yystack_.begin(), i_end = yystack_.end();
       i != i_end; ++i)
    *yycdebug_ << ' ' << i->state;
  *yycdebug_ << std::endl;
}

// Report on the debug stream that the rule \a yyrule is going to be reduced.
void SVGPathParser::yy_reduce_print_(int yyrule) {
  unsigned int yylno = yyrline_[yyrule];
  int yynrhs = yyr2_[yyrule];
  // Print the symbols being reduced, and their result.
  *yycdebug_ << "Reducing stack by rule " << yyrule - 1 << " (line " << yylno
             << "):" << std::endl;
  // The symbols being reduced.
  for (int yyi = 0; yyi < yynrhs; yyi++)
    YY_SYMBOL_PRINT("   $" << yyi + 1 << " =", yystack_[(yynrhs) - (yyi + 1)]);
}
#endif  // YYDEBUG

}  // rl

void rl::SVGPathParser::error(const rl::SVGPathParser::location_type& loc,
                              const std::string& message) {
  driver.error(loc, message);
}
