#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
HeyAakash
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();               //   and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 2.0 : 01 Jun 11
- added processing on way to building strong code analyzer
ver 1.1 : 17 Jan 09
- changed to accept a pointer to interfaced ITokCollection instead
of a SemiExpression
ver 1.0 : 12 Jan 06
- first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "MNode.h"
#include "MTree.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

struct element
{
  std::string type;
  std::string name;
  size_t totalLines;
  size_t lineCount;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << lineCount;
    temp << ")";
    return temp.str();
  }
};

typedef MNode<element> Node;
//static Node* pRoot;

//static int complexity = 0;

class newOper : public Operation<Node>
{
public:
  bool operator()(Node* pNode)
  {
    //std::cout << "\n -- " << pNode->getNode().type << " " << pNode->getNode().name << " " << pNode->getScope();
    return false;
  }
};

//static MTree<Node> Tree(new newOper);

//static std::vector<Node*> NodeList;
//static std::vector<Node*> RootList;


///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
  ScopeStack<element> stack;
  Toker* p_Toker;
  Node* pRoot;
  Node* MaxNode;
  int complexity;
  std::vector<Node*> NodeList;
  std::vector<Node*> RootList;

public:

  void pushNodeList(Node* node)
  {
    NodeList.push_back(node);
  }

  void popNodeList()
  {
    NodeList.pop_back();
  }

  Node* getLastNode()
  {
    return NodeList.back();
  }

  void pushRootList(Node* node)
  {
    RootList.push_back(node);
  }
  /*std::vector<Node*> getRootList()
  {
  return RootList;
  }*/

  int getComplexity()
  {
    return complexity;
  }

  void setComplexity(int complexityCount)
  {
    complexity = complexityCount;
  }

  Node* getRoot()
  {
    return pRoot;
  }

  void setRoot(Node* root)
  {
    pRoot = root;
  }

  Repository(Toker* pToker)
  {
    p_Toker = pToker;
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->lines());
  }

  int getNodeListSize()
  {
    return NodeList.size();
  }

  void getMaxNode()
  {
    Node* TempNode = RootList.front()->clone();
    for (std::vector<Node*>::size_type i = 0; i != RootList.size(); i++)
    {
      if (TempNode->getTotal() < RootList[i]->getTotal())
        TempNode = RootList[i];
    }
    MaxNode = TempNode->clone();
  }

  void nodeToXML(Node* node)
  {
    std::cout << "\n";
    for (int i = 0; i < node->getScope(); i++)
      std::cout << "\t";
    std::cout << "<" << node->getNode().type << " name='" << node->getNode().name << "'>";
    std::vector<Node*> children = node->getChildren();
    for (auto child : children)
    {
      nodeToXML(child);
    }
    std::cout << "\n";
    for (int i = 0; i < node->getScope(); i++)
      std::cout << "\t";
    std::cout << "<" << node->getNode().type << ">";
  }

  void printTree()
  {
    std::cout << "\n Maximum Scope Node : \n";
    getMaxNode();
    nodeToXML(MaxNode);
    for (std::vector<Node*>::size_type i = 0; i != RootList.size(); i++)
    {
      MTree<Node> Tree(new newOper);
      Tree.makeRoot(RootList[i]);
      Tree.verbose() = true;
      Tree.walk();
      //std::cout << "\nComplexity : " << RootList[i]->getTotal() << "\n\n";
    }

    MTree<Node> Tree(new newOper);
    Tree.makeRoot(MaxNode);
    Tree.verbose() = true;
    Tree.walk();
  }

};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {

    //std::cout << "\n--BeginningOfScope rule";
    if (pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if (pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    if (p_Repos->scopeStack().size() == 0)
      return;
    element elem = p_Repos->scopeStack().pop();
    if (elem.type == "function")
    {
      //std::cout << "\n  Function " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
      elem.totalLines = p_Repos->lineCount() - elem.lineCount + 1;
      Node* temp = p_Repos->getRoot();
      temp->setTotal(p_Repos->getComplexity());
      p_Repos->pushRootList(temp);
      p_Repos->popNodeList();
      //MTree<Node> Tree(new newOper);
      //temp->setTotal(complexit y);
      //Tree.makeRoot(temp);
      //Tree.verbose() = true;
      //Tree.setTotalNodes(complexity);
      ////TreeList.push_back(Tree);
      //NodeList.pop_back();
    }
    else if (elem.type == "class")
    {
      //std::cout << "\n  Class " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
      elem.totalLines = p_Repos->lineCount() - elem.lineCount + 1;
    }
    else if (elem.type == "struct")
    {
      //std::cout << "\n  Struct " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
      elem.totalLines = p_Repos->lineCount() - elem.lineCount + 1;
    }
    else if (elem.type == "enum")
    {
      //std::cout << "\n  Enum " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
      elem.totalLines = p_Repos->lineCount() - elem.lineCount + 1;
    }
    else if (elem.type == "special")
    {
      p_Repos->popNodeList();
      elem.totalLines = p_Repos->lineCount() - elem.lineCount + 1;
    }

  }
  //    if (p_Repos->scopeStack().size() == 0)
  //    {
  //      for (std::vector<MTree<Node>>::size_type i = 0; i != TreeList.size(); i++)
  //      {
  //                  MTree<Node> Tree(new newOper);
  //          Tree.makeRoot(pRoot);
  //          Tree.verbose() = true;
  //          //TreeList.push_back(Tree);
  //          Tree.walk();
  //
  ////        TreeList[i].walk();
  //        //TreeList.clear();
  //      //}
  //    }

};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--PreprocStatement rule";
    if (pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch", "try" };
    for (int i = 0; i < 6; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {

    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
      {
        //if (pRoot != NULL)
        //{
        //  MTree<Node> Tree(new newOper);
        //  Tree.makeRoot(pRoot);
        //  Tree.verbose() = true;
        //  TreeList.push_back(Tree);
        //  Tree.walk();
        //}

        //std::cout << "\n--FunctionDefinition rule";
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
    p_Repos->setRoot(new Node(elem));
    p_Repos->pushNodeList(p_Repos->getRoot());
    p_Repos->setComplexity(0);
  }
};


///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for (int i = 0; i<len + 1; ++i)
      std::cout << (*pTc)[i] << " ";
  }
};


///////////////////////////////////////////////////////////////
// rule to detect special keyword definitions

class SpecialKeywordDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch", "else if" };
    for (int i = 0; i < 5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
      {
        //std::cout << "\n--FunctionDefinition rule";
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to push special keyword name onto ScopeStack

class PushSpecialKeyword : public IAction
{
  Repository* p_Repos;
public:
  PushSpecialKeyword(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "special";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
    //pRoot = new Node(elem);
    Node* tempNode = new Node(elem);
    tempNode->setScope(p_Repos->getNodeListSize());
    p_Repos->getLastNode()->addChild(tempNode);
    //p_Repos->getLastNode()->setComplexity();
    p_Repos->pushNodeList(tempNode);
    p_Repos->setComplexity(p_Repos->getComplexity() + 1);
  }
};

class ElseDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("else");
      if (len < tc.length())
      {
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};


class PushElse : public IAction
{
  Repository* p_Repos;
public:
  PushElse(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    element elem;
    elem.type = "special";
    elem.name = "else";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
    //pRoot = new Node(elem);
    Node* tempNode = new Node(elem);
    tempNode->setScope(p_Repos->getNodeListSize());
    p_Repos->getLastNode()->addChild(tempNode);
    //p_Repos->getLastNode()->setComplexity();
    p_Repos->pushNodeList(tempNode);
    p_Repos->setComplexity(p_Repos->getComplexity() + 1);
  }
};

class TryDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("try");
      if (len < tc.length())
      {
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};


class PushTry : public IAction
{
  Repository* p_Repos;
public:
  PushTry(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    element elem;
    elem.type = "special";
    elem.name = "try";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
    //pRoot = new Node(elem);
    Node* tempNode = new Node(elem);
    tempNode->setScope(p_Repos->getNodeListSize());
    p_Repos->getLastNode()->addChild(tempNode);
    //p_Repos->getLastNode()->setComplexity();
    p_Repos->pushNodeList(tempNode);
    p_Repos->setComplexity(p_Repos->getComplexity() + 1);
  }
};

//test
class ClassDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("class");
      if (len < tc.length())
      {
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};


class PushClass : public IAction
{
  Repository* p_Repos;
public:
  PushClass(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("class") + 1];
    element elem;
    elem.type = "class";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};

class StructDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("struct");
      if (len < tc.length())
      {
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};


class PushStruct : public IAction
{
  Repository* p_Repos;
public:
  PushStruct(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("struct") + 1];
    element elem;
    elem.type = "struct";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};

class EnumDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("enum");
      if (len < tc.length())
      {
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};


class PushEnum : public IAction
{
  Repository* p_Repos;
public:
  PushEnum(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("enum") + 1];
    element elem;
    elem.type = "enum";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};

class NamespaceDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("namespace");
      if (len < tc.length())
      {
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};


class PushNamespace : public IAction
{
  Repository* p_Repos;
public:
  PushNamespace(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("namespace") + 1];
    element elem;
    elem.type = "namespace";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};



#endif
