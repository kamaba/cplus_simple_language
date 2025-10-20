#include "FileMetatUtil.h"
#include "FileMeta/FileMeta.h"
#include "FileMeta/FileMetaExpress.h"
#include "../Core/Log.h"
#include "../Core/GrammerUtil.h"
#include "../Core/StructParse.h"
#include "../Core/SignComputePriority.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

std::vector<std::string> FileMetatUtil::GetLinkStringMidPeriodList(const std::vector<Token*>& tokenList) {
    std::vector<std::string> stringList;
    for (size_t i = 0; i < tokenList.size(); i++) {
        auto token = tokenList[i];
        if (token->lexeme().IsNull()) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Core::EError::None, "检查到Import语句中，token内容lexeme为空!!");
            return std::vector<std::string>();
        }
        if (token->type() != SimpleLanguage::Core::ETokenType::Period) {
            if (!SimpleLanguage::Core::GrammerUtil::IdentifierCheck(token->lexeme().ToString())) {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Core::EError::None, "检查到Import语句中，导入名称不合规!!");
                return std::vector<std::string>();
            }
            stringList.push_back(token->lexeme().ToString());
        }
    }
    return stringList;
}

bool FileMetatUtil::IsSymbol(Token* token) {
    switch (token->type()) {
        case SimpleLanguage::Core::ETokenType::Plus:
        case SimpleLanguage::Core::ETokenType::Minus:
        case SimpleLanguage::Core::ETokenType::Multiply:
        case SimpleLanguage::Core::ETokenType::Divide:
        case SimpleLanguage::Core::ETokenType::DoublePlus:     //++
        case SimpleLanguage::Core::ETokenType::DoubleMinus:    //--
        case SimpleLanguage::Core::ETokenType::Modulo:          // %
        case SimpleLanguage::Core::ETokenType::Not:             // !
        case SimpleLanguage::Core::ETokenType::Negative:        // ~
        case SimpleLanguage::Core::ETokenType::Shi:               //  <<
        case SimpleLanguage::Core::ETokenType::Shr:               //  >>
        case SimpleLanguage::Core::ETokenType::Less:            // >
        case SimpleLanguage::Core::ETokenType::GreaterOrEqual:  // >=
        case SimpleLanguage::Core::ETokenType::Greater:         // <
        case SimpleLanguage::Core::ETokenType::LessOrEqual:     // <=
        case SimpleLanguage::Core::ETokenType::Equal:           // ==
        case SimpleLanguage::Core::ETokenType::NotEqual:        // !=
        case SimpleLanguage::Core::ETokenType::Combine:         // &
        case SimpleLanguage::Core::ETokenType::InclusiveOr:     // |
        case SimpleLanguage::Core::ETokenType::XOR:             //  ^
        case SimpleLanguage::Core::ETokenType::Or:              // ||
        case SimpleLanguage::Core::ETokenType::And:             // &&  
        case SimpleLanguage::Core::ETokenType::PlusAssign:             // +=
        case SimpleLanguage::Core::ETokenType::MinusAssign:            // -=
        case SimpleLanguage::Core::ETokenType::MultiplyAssign:         // *=
        case SimpleLanguage::Core::ETokenType::DivideAssign:           // /=
        case SimpleLanguage::Core::ETokenType::ModuloAssign:           // %=
        case SimpleLanguage::Core::ETokenType::InclusiveOrAssign:      // |=
        case SimpleLanguage::Core::ETokenType::XORAssign:              // ^=
            return true;
    }
    return false;
}

bool FileMetatUtil::SplitNodeList(const std::vector<Node*>& nodeList, std::vector<Node*>& preNodeList, 
                                   std::vector<Node*>& afterNodeList, Token*& assignToken) {
    bool isEqual = false;
    for (size_t i = 0; i < nodeList.size(); i++) {
        auto n = nodeList[i];
        if (n->token()->type() == SimpleLanguage::Core::ETokenType::Assign) {
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
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Core::EError::None, "解析NodeStructVariable时有=号，但没有值内容 " + (assignToken ? assignToken->ToLexemeAllString() : ""));
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
    if (node->nodeType() == SimpleLanguage::Core::ENodeType::IdentifierLink ||
        (node->nodeType() == SimpleLanguage::Core::ENodeType::Key && 
         (node->token() != nullptr && 
          (node->token()->type() == SimpleLanguage::Core::ETokenType::This || 
           node->token()->type() == SimpleLanguage::Core::ETokenType::Base)))) {
        fmbt = new FileMetaCallTerm(fm, node);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else if (node->nodeType() == SimpleLanguage::Core::ENodeType::ConstValue) {
        if (node->extendLinkNodeList().size() > 1) {
            fmbt = new FileMetaCallTerm(fm, node);
            fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
        } else {
            fmbt = new FileMetaConstValueTerm(fm, node->token());
            fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
        }
    } else if (node->nodeType() == SimpleLanguage::Core::ENodeType::Par) {
        fmbt = new FileMetaParTerm(fm, node, expressType);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else if (node->nodeType() == SimpleLanguage::Core::ENodeType::Brace) {
        fmbt = new FileMetaBraceTerm(fm, node);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else if (node->nodeType() == SimpleLanguage::Core::ENodeType::Bracket) {
        fmbt = new FileMetaBracketTerm(fm, node);
        fmbt->priority = SimpleLanguage::Core::SignComputePriority::Level1;
    } else {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Core::EError::None, "Error CreateFileOneTerm 单1表达式，没有找到该类型: " + 
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
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Core::EError::None, "Error 生成表达式错误!!");
        return nullptr;
    }
    fmbt->BuildAST();
    return fmbt;
}

} // namespace Compile
} // namespace SimpleLanguage

