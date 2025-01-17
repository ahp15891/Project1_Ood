/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.0                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

//----< destructor releases all parts >------------------------------
HeyAakash
ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPushFunction;
  delete pFunctionDefinition;

  delete pPushElse;
  delete pElseDefinition;

  delete pPushTry;
  delete pTryDefinition;

  delete pPushClass;
  delete pClassDefinition;

  delete pPushStruct;
  delete pStructDefinition;

  delete pPushEnum;
  delete pEnumDefinition;

  delete pPushNamespace;
  delete pNamespaceDefinition;

  delete pFR;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if (pToker == 0)
    return false;
  return pToker->attach(name, isFile);
}
//----< Here's where alll the parts get assembled >----------------


Parser* ConfigParseToConsole::print()
{
  pRepo->printTree();
  //std::cout << "Hellofsjfhjfhfdjfh";
  return 0;
}
Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComments();
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);




    // add code folding rules

    pFR = new codeFoldingRules;
    pParser->addFoldingRules(pFR);

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);

    // configure to detect and act on function definitions
    // these will stop further rule checking by returning false



    pClassDefinition = new ClassDefinition;
    pPushClass = new PushClass(pRepo);  // no action
    pClassDefinition->addAction(pPushClass);
    pParser->addRule(pClassDefinition);

    pStructDefinition = new StructDefinition;
    pPushStruct = new PushStruct(pRepo);  // no action
    pStructDefinition->addAction(pPushStruct);
    pParser->addRule(pStructDefinition);

    pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);  // no action
    pFunctionDefinition->addAction(pPushFunction);
    pParser->addRule(pFunctionDefinition);

    pSpecialKeywordDefinition = new SpecialKeywordDefinition;
    pPushSpecialKeyword = new PushSpecialKeyword(pRepo);  // no action
    pSpecialKeywordDefinition->addAction(pPushSpecialKeyword);
    pParser->addRule(pSpecialKeywordDefinition);

    pElseDefinition = new ElseDefinition;
    pPushElse = new PushElse(pRepo);  // no action
    pElseDefinition->addAction(pPushElse);
    pParser->addRule(pElseDefinition);

    pTryDefinition = new TryDefinition;
    pPushTry = new PushTry(pRepo);  // no action
    pTryDefinition->addAction(pPushTry);
    pParser->addRule(pTryDefinition);

    pEnumDefinition = new EnumDefinition;
    pPushEnum = new PushEnum(pRepo);  // no action
    pEnumDefinition->addAction(pPushEnum);
    pParser->addRule(pEnumDefinition);

    pNamespaceDefinition = new NamespaceDefinition;
    pPushNamespace = new PushNamespace(pRepo);  // no action
    pNamespaceDefinition->addAction(pPushNamespace);
    pParser->addRule(pNamespaceDefinition);

    return pParser;
  }
  catch (std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
    << std::string(32, '=') << std::endl;

  // collecting tokens from files, named on the command line

  if (argc < 2)
  {
    std::cout
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for (int i = 1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if (pParser)
      {
        if (!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while (pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch (std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
