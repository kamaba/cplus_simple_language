#include "FileMetatUtil.h"
#include "FileMeta/FileMeta.h"
#include "FileMeta/FileMetaExpress.h"
#include "../Debug/Log.h"
//#include "../Core/GrammerUtil.h"
#include "Parse/StructParse.h"
#include "Parse/Node.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

std::vector<std::string> FileMetatUtil::GetLinkStringMidPeriodList(const std::vector<Token*>& tokenList) {
    std::vector<std::string> stringList;
    for (size_t i = 0; i < tokenList.size(); i++) {
        auto token = tokenList[i];
        if (token->GetLexeme() == "" ) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "检查到Import语句中，token内容lexeme为空!!");
            return std::vector<std::string>();
        }
        if (token->GetType() != SimpleLanguage::ETokenType::Period) {
            if (!SimpleLanguage::Core::GrammerUtil::IdentifierCheck( token->GetLexeme() ) {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "检查到Import语句中，导入名称不合规!!");
                return std::vector<std::string>();
            }
            stringList.push_back(token->lexeme().ToString());
        }
    }
    return stringList;
}

bool FileMetatUtil::IsSymbol(Token* token) {
    switch (token->GetType()) {
        case SimpleLanguage::ETokenType::Plus:
        case SimpleLanguage::ETokenType::Minus:
        case SimpleLanguage::ETokenType::Multiply:
        case SimpleLanguage::ETokenType::Divide:
        case SimpleLanguage::ETokenType::DoublePlus:     //++
        case SimpleLanguage::ETokenType::DoubleMinus:    //--
        case SimpleLanguage::ETokenType::Modulo:          // %
        case SimpleLanguage::ETokenType::Not:             // !
        case SimpleLanguage::ETokenType::Negative:        // ~
        case SimpleLanguage::ETokenType::Shi:               //  <<
        case SimpleLanguage::ETokenType::Shr:               //  >>
        case SimpleLanguage::ETokenType::Less:            // >
        case SimpleLanguage::ETokenType::GreaterOrEqual:  // >=
        case SimpleLanguage::ETokenType::Greater:         // <
        case SimpleLanguage::ETokenType::LessOrEqual:     // <=
        case SimpleLanguage::ETokenType::Equal:           // ==
        case SimpleLanguage::ETokenType::NotEqual:        // !=
        case SimpleLanguage::ETokenType::Combine:         // &
        case SimpleLanguage::ETokenType::InclusiveOr:     // |
        case SimpleLanguage::ETokenType::XOR:             //  ^
        case SimpleLanguage::ETokenType::Or:              // ||
        case SimpleLanguage::ETokenType::And:             // &&  
        case SimpleLanguage::ETokenType::PlusAssign:             // +=
        case SimpleLanguage::ETokenType::MinusAssign:            // -=
        case SimpleLanguage::ETokenType::MultiplyAssign:         // *=
        case SimpleLanguage::ETokenType::DivideAssign:           // /=
        case SimpleLanguage::ETokenType::ModuloAssign:           // %=
        case SimpleLanguage::ETokenType::InclusiveOrAssign:      // |=
        case SimpleLanguage::ETokenType::XORAssign:              // ^=
            return true;
    }
    return false;
}

bool FileMetatUtil::SplitNodeList(const std::vector<Node*>& nodeList, std::vector<Node*>& preNodeList, 
                                   std::vector<Node*>& afterNodeList, Token*& assignToken) {
    bool isEqual = false;
    for (size_t i = 0; i < nodeList.size(); i++) {
        auto n = nodeList[i];
        if (n->token()->type() == SimpleLanguage::ETokenType::Assign) {
            isEqual = true;
            assignToken = n->token();
            continue;
        }
        if (isEqual)
            afterNodeList.push_back(n);
        else
            preNodeList.push_back(n);
    }
    if (isEqual) {
        if (afterNodeList.empty()) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "解析NodeStructVariable时有=号，但没有值内容 " + (assignToken ? assignToken->ToLexemeAllString() : ""));
            return false;
        }
    }
    if (preNodeList.empty()) {
        return false;
    }
    return true;
}

FileMetaBaseTerm* FileMetatUtil::CreateFileOneTerm(FileMeta* fm, Node* node, FileMetaTermExpress::EExpressType expressType) {
    FileMetaBaseTerm* fmbt = nullptr;
    if (node->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::IdentifierLink ||
        (node->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::Key && 
         (node->token() != nullptr && 
          (node->token()->type() == SimpleLanguage::ETokenType::This || 
           node->token()->type() == SimpleLanguage::ETokenType::Base)))) {
        fmbt = new FileMetaCallTerm(fm, node);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else if (node->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::ConstValue) {
        if (node->extendLinkNodeList().size() > 1) {
            fmbt = new FileMetaCallTerm(fm, node);
            fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
        } else {
            fmbt = new FileMetaConstValueTerm(fm, node->token());
            fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
        }
    } else if (node->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::Par) {
        fmbt = new FileMetaParTerm(fm, node, expressType);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else if (node->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::Brace) {
        fmbt = new FileMetaBraceTerm(fm, node);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else if (node->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::Bracket) {
        fmbt = new FileMetaBracketTerm(fm, node);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error CreateFileOneTerm 单1表达式，没有找到该类型: " + 
            (node->token() ? std::to_string(static_cast<int>(node->token()->type())) : "null") + " 位置: " + 
            (node->token() ? node->token()->ToLexemeAllString() : ""));
    }
    return fmbt;
}

FileMetaBaseTerm* FileMetatUtil::CreateFileMetaExpress(FileMeta* fm, const std::vector<Node*>& nodeList, 
                                                       FileMetaTermExpress::EExpressType expressType) {
    if (nodeList.empty())
        return nullptr;

    FileMetaBaseTerm* fmbt = nullptr;
    if (nodeList.size() == 1) {
        fmbt = CreateFileOneTerm(fm, nodeList[0], expressType);
    } else {
        fmbt = new FileMetaTermExpress(fm, nodeList, expressType);
    }
    if (fmbt == nullptr) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 生成表达式错误!!");
        return nullptr;
    }
    fmbt->BuildAST();
    return fmbt;
}

} // namespace Compile
} // namespace SimpleLanguage

