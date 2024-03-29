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
#define yylex RLSVGXForm_lex

// First part of user declarations.

#ifndef YY_NULLPTR
#if defined __cplusplus && 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#endif

#include "SVGXFormParser.h"

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
std::string SVGXFormParser::yytnamerr_(const char* yystr) {
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
SVGXFormParser::SVGXFormParser(rl::SVGXFormDriver& driver_yyarg,
                               void* scanner_yyarg)
    :
#if YYDEBUG
      yydebug_(false),
      yycdebug_(&std::cerr),
#endif
      driver(driver_yyarg),
      scanner(scanner_yyarg) {
}

SVGXFormParser::~SVGXFormParser() {}

/*---------------.
| Symbol types.  |
`---------------*/

// by_state.
inline SVGXFormParser::by_state::by_state() : state(empty_state) {}

inline SVGXFormParser::by_state::by_state(const by_state& other)
    : state(other.state) {}

inline void SVGXFormParser::by_state::clear() {
  state = empty_state;
}

inline void SVGXFormParser::by_state::move(by_state& that) {
  state = that.state;
  that.clear();
}

inline SVGXFormParser::by_state::by_state(state_type s) : state(s) {}

inline SVGXFormParser::symbol_number_type SVGXFormParser::by_state::type_get()
    const {
  if (state == empty_state)
    return empty_symbol;
  else
    return yystos_[state];
}

inline SVGXFormParser::stack_symbol_type::stack_symbol_type() {}

inline SVGXFormParser::stack_symbol_type::stack_symbol_type(state_type s,
                                                            symbol_type& that)
    : super_type(s, that.location) {
  switch (that.type_get()) {
    case 12:  // "<number>"
      value.move<rl::SVGNumber>(that.value);
      break;

    case 16:  // Matrix
      value.move<rl::SVGXFormMatrix>(that.value);
      break;

    case 19:  // Rotate
      value.move<rl::SVGXFormRotate>(that.value);
      break;

    case 18:  // Scale
      value.move<rl::SVGXFormScale>(that.value);
      break;

    case 20:  // SkewX
      value.move<rl::SVGXFormSkewX>(that.value);
      break;

    case 21:  // SkewY
      value.move<rl::SVGXFormSkewY>(that.value);
      break;

    case 17:  // Translate
      value.move<rl::SVGXFormTranslate>(that.value);
      break;

    default:
      break;
  }

  // that is emptied.
  that.type = empty_symbol;
}

inline SVGXFormParser::stack_symbol_type& SVGXFormParser::stack_symbol_type::
operator=(const stack_symbol_type& that) {
  state = that.state;
  switch (that.type_get()) {
    case 12:  // "<number>"
      value.copy<rl::SVGNumber>(that.value);
      break;

    case 16:  // Matrix
      value.copy<rl::SVGXFormMatrix>(that.value);
      break;

    case 19:  // Rotate
      value.copy<rl::SVGXFormRotate>(that.value);
      break;

    case 18:  // Scale
      value.copy<rl::SVGXFormScale>(that.value);
      break;

    case 20:  // SkewX
      value.copy<rl::SVGXFormSkewX>(that.value);
      break;

    case 21:  // SkewY
      value.copy<rl::SVGXFormSkewY>(that.value);
      break;

    case 17:  // Translate
      value.copy<rl::SVGXFormTranslate>(that.value);
      break;

    default:
      break;
  }

  location = that.location;
  return *this;
}

template <typename Base>
inline void SVGXFormParser::yy_destroy_(const char* yymsg,
                                        basic_symbol<Base>& yysym) const {
  if (yymsg)
    YY_SYMBOL_PRINT(yymsg, yysym);
}

#if YYDEBUG
template <typename Base>
void SVGXFormParser::yy_print_(std::ostream& yyo,
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

inline void SVGXFormParser::yypush_(const char* m,
                                    state_type s,
                                    symbol_type& sym) {
  stack_symbol_type t(s, sym);
  yypush_(m, t);
}

inline void SVGXFormParser::yypush_(const char* m, stack_symbol_type& s) {
  if (m)
    YY_SYMBOL_PRINT(m, s);
  yystack_.push(s);
}

inline void SVGXFormParser::yypop_(unsigned int n) {
  yystack_.pop(n);
}

#if YYDEBUG
std::ostream& SVGXFormParser::debug_stream() const {
  return *yycdebug_;
}

void SVGXFormParser::set_debug_stream(std::ostream& o) {
  yycdebug_ = &o;
}

SVGXFormParser::debug_level_type SVGXFormParser::debug_level() const {
  return yydebug_;
}

void SVGXFormParser::set_debug_level(debug_level_type l) {
  yydebug_ = l;
}
#endif  // YYDEBUG

inline SVGXFormParser::state_type SVGXFormParser::yy_lr_goto_state_(
    state_type yystate,
    int yysym) {
  int yyr = yypgoto_[yysym - yyntokens_] + yystate;
  if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
    return yytable_[yyr];
  else
    return yydefgoto_[yysym - yyntokens_];
}

inline bool SVGXFormParser::yy_pact_value_is_default_(int yyvalue) {
  return yyvalue == yypact_ninf_;
}

inline bool SVGXFormParser::yy_table_value_is_error_(int yyvalue) {
  return yyvalue == yytable_ninf_;
}

int SVGXFormParser::parse() {
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
        case 12:  // "<number>"
          yylhs.value.build<rl::SVGNumber>();
          break;

        case 16:  // Matrix
          yylhs.value.build<rl::SVGXFormMatrix>();
          break;

        case 19:  // Rotate
          yylhs.value.build<rl::SVGXFormRotate>();
          break;

        case 18:  // Scale
          yylhs.value.build<rl::SVGXFormScale>();
          break;

        case 20:  // SkewX
          yylhs.value.build<rl::SVGXFormSkewX>();
          break;

        case 21:  // SkewY
          yylhs.value.build<rl::SVGXFormSkewY>();
          break;

        case 17:  // Translate
          yylhs.value.build<rl::SVGXFormTranslate>();
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
            driver.visitElement(yystack_[0].value.as<rl::SVGXFormMatrix>());
          }

          break;

          case 5:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGXFormTranslate>());
          }

          break;

          case 6:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGXFormScale>());
          }

          break;

          case 7:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGXFormRotate>());
          }

          break;

          case 8:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGXFormSkewX>());
          }

          break;

          case 9:

          {
            driver.visitElement(yystack_[0].value.as<rl::SVGXFormSkewY>());
          }

          break;

          case 10:

          {
            yylhs.value.as<rl::SVGXFormMatrix>() = {
                yystack_[6].value.as<rl::SVGNumber>(),
                yystack_[5].value.as<rl::SVGNumber>(),
                yystack_[4].value.as<rl::SVGNumber>(),
                yystack_[3].value.as<rl::SVGNumber>(),
                yystack_[2].value.as<rl::SVGNumber>(),
                yystack_[1].value.as<rl::SVGNumber>(),
            };
          }

          break;

          case 11:

          {
            yylhs.value.as<rl::SVGXFormTranslate>() = {
                yystack_[1].value.as<rl::SVGNumber>(), 0.0};
          }

          break;

          case 12:

          {
            yylhs.value.as<rl::SVGXFormTranslate>() = {
                yystack_[2].value.as<rl::SVGNumber>(),
                yystack_[1].value.as<rl::SVGNumber>()};
          }

          break;

          case 13:

          {
            yylhs.value.as<rl::SVGXFormScale>() = {
                yystack_[1].value.as<rl::SVGNumber>(),
                yystack_[1].value.as<rl::SVGNumber>()};
          }

          break;

          case 14:

          {
            yylhs.value.as<rl::SVGXFormScale>() = {
                yystack_[2].value.as<rl::SVGNumber>(),
                yystack_[1].value.as<rl::SVGNumber>()};
          }

          break;

          case 15:

          {
            yylhs.value.as<rl::SVGXFormRotate>() = {
                yystack_[1].value.as<rl::SVGNumber>()};
          }

          break;

          case 16:

          {
            yylhs.value.as<rl::SVGXFormRotate>() = {
                yystack_[3].value.as<rl::SVGNumber>(),
                {yystack_[2].value.as<rl::SVGNumber>(),
                 yystack_[1].value.as<rl::SVGNumber>()}};
          }

          break;

          case 17:

          {
            yylhs.value.as<rl::SVGXFormSkewX>() = {
                yystack_[1].value.as<rl::SVGNumber>()};
          }

          break;

          case 18:

          {
            yylhs.value.as<rl::SVGXFormSkewY>() = {
                yystack_[1].value.as<rl::SVGNumber>()};
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

void SVGXFormParser::error(const syntax_error& yyexc) {
  error(yyexc.location, yyexc.what());
}

// Generate an error message.
std::string SVGXFormParser::yysyntax_error_(state_type yystate,
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

const signed char SVGXFormParser::yypact_ninf_ = -11;

const signed char SVGXFormParser::yytable_ninf_ = -1;

const signed char SVGXFormParser::yypact_[] = {
    6,   -7, 10,  11, 12,  13,  14,  0,   -11, -11, -11, -11, -11, -11, -11, 15,
    16,  17, 18,  19, 20,  -11, -11, 21,  -10, 5,   7,   23,  24,  25,  -11, 27,
    -11, 28, -11, 29, -11, -11, 30,  -11, -11, 32,  33,  -11, 34,  36,  -11};

const unsigned char SVGXFormParser::yydefact_[] = {
    0,  0, 0,  0, 0,  0,  0, 0,  2,  4, 5, 6,  7, 8, 9,  0,
    0,  0, 0,  0, 0,  1,  3, 0,  0,  0, 0, 0,  0, 0, 11, 0,
    13, 0, 15, 0, 17, 18, 0, 12, 14, 0, 0, 16, 0, 0, 10};

const signed char SVGXFormParser::yypgoto_[] = {-11, -11, 37,  -11, -11,
                                                -11, -11, -11, -11};

const signed char SVGXFormParser::yydefgoto_[] = {-1, 7,  8,  9, 10,
                                                  11, 12, 13, 14};

const unsigned char SVGXFormParser::yytable_[] = {
    21, 30, 31, 15, 1,  2,  3,  4,  5,  6,  1,  2,  3,  4,  5,  6,
    32, 33, 34, 35, 16, 17, 18, 19, 20, 0,  0,  23, 24, 25, 26, 27,
    28, 29, 36, 37, 0,  38, 39, 40, 0,  41, 42, 43, 22, 44, 45, 46};

const signed char SVGXFormParser::yycheck_[] = {
    0,  11, 12, 10, 4,  5,  6,  7,  8,  9,  4,  5,  6,  7,  8,  9,
    11, 12, 11, 12, 10, 10, 10, 10, 10, -1, -1, 12, 12, 12, 12, 12,
    12, 12, 11, 11, -1, 12, 11, 11, -1, 12, 12, 11, 7,  12, 12, 11};

const unsigned char SVGXFormParser::yystos_[] = {
    0,  4,  5,  6,  7,  8,  9,  14, 15, 16, 17, 18, 19, 20, 21, 10,
    10, 10, 10, 10, 10, 0,  15, 12, 12, 12, 12, 12, 12, 12, 11, 12,
    11, 12, 11, 12, 11, 11, 12, 11, 11, 12, 12, 11, 12, 12, 11};

const unsigned char SVGXFormParser::yyr1_[] = {
    0, 13, 14, 14, 15, 15, 15, 15, 15, 15, 16, 17, 17, 18, 18, 19, 19, 20, 21};

const unsigned char SVGXFormParser::yyr2_[] = {0, 2, 1, 2, 1, 1, 1, 1, 1, 1,
                                               9, 4, 5, 4, 5, 4, 6, 4, 4};

// YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
// First, the terminals, then, starting at \a yyntokens_, nonterminals.
const char* const SVGXFormParser::yytname_[] = {"\"<end of contents>\"",
                                                "error",
                                                "$undefined",
                                                "\"<invalid token>\"",
                                                "\"Matrix\"",
                                                "\"Translate\"",
                                                "\"Scale\"",
                                                "\"Rotate\"",
                                                "\"Skew X\"",
                                                "\"Skew Y\"",
                                                "\"(\"",
                                                "\")\"",
                                                "\"<number>\"",
                                                "$accept",
                                                "XFormElements",
                                                "XFormElement",
                                                "Matrix",
                                                "Translate",
                                                "Scale",
                                                "Rotate",
                                                "SkewX",
                                                "SkewY",
                                                YY_NULLPTR};

#if YYDEBUG
const unsigned char SVGXFormParser::yyrline_[] = {
    0, 64, 64, 65, 69, 70, 71, 72, 73, 74, 78, 82, 83, 87, 88, 92, 93, 97, 101};

// Print the state stack on the debug stream.
void SVGXFormParser::yystack_print_() {
  *yycdebug_ << "Stack now";
  for (stack_type::const_iterator i = yystack_.begin(), i_end = yystack_.end();
       i != i_end; ++i)
    *yycdebug_ << ' ' << i->state;
  *yycdebug_ << std::endl;
}

// Report on the debug stream that the rule \a yyrule is going to be reduced.
void SVGXFormParser::yy_reduce_print_(int yyrule) {
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

void rl::SVGXFormParser::error(const rl::SVGXFormParser::location_type& loc,
                               const std::string& message) {
  driver.error(loc, message);
}
