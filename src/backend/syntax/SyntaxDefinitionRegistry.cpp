#include "SyntaxDefinitionRegistry.hpp"

namespace backend {

const SyntaxDefinition &SyntaxDefinitionRegistry::cDefinition()
{
    static const SyntaxDefinition definition = {
        SyntaxLanguage::C,
        QStringLiteral("c"),
        {QStringLiteral(".c"), QStringLiteral(".h")},
        {
            QStringLiteral("break"),    QStringLiteral("case"),     QStringLiteral("continue"),
            QStringLiteral("default"),  QStringLiteral("do"),       QStringLiteral("else"),
            QStringLiteral("for"),      QStringLiteral("goto"),     QStringLiteral("if"),
            QStringLiteral("return"),   QStringLiteral("switch"),   QStringLiteral("while"),
        },
        {
            QStringLiteral("_Bool"),    QStringLiteral("_Complex"), QStringLiteral("_Imaginary"),
            QStringLiteral("auto"),     QStringLiteral("char"),     QStringLiteral("const"),
            QStringLiteral("double"),   QStringLiteral("enum"),     QStringLiteral("extern"),
            QStringLiteral("float"),    QStringLiteral("inline"),   QStringLiteral("int"),
            QStringLiteral("long"),     QStringLiteral("register"), QStringLiteral("restrict"),
            QStringLiteral("short"),    QStringLiteral("signed"),   QStringLiteral("sizeof"),
            QStringLiteral("static"),   QStringLiteral("struct"),   QStringLiteral("typedef"),
            QStringLiteral("union"),    QStringLiteral("unsigned"), QStringLiteral("void"),
            QStringLiteral("volatile"), QStringLiteral("bool"),     QStringLiteral("false"),
            QStringLiteral("true"),
        },
        true,
        true,
    };
    return definition;
}

const SyntaxDefinition &SyntaxDefinitionRegistry::cppDefinition()
{
    static const SyntaxDefinition definition = {
        SyntaxLanguage::Cpp,
        QStringLiteral("cpp"),
        {
            QStringLiteral(".cpp"), QStringLiteral(".cxx"), QStringLiteral(".cc"),
            QStringLiteral(".hpp"), QStringLiteral(".hxx"), QStringLiteral(".hh"),
        },
        {
            QStringLiteral("break"),    QStringLiteral("case"),     QStringLiteral("catch"),
            QStringLiteral("continue"), QStringLiteral("default"),  QStringLiteral("do"),
            QStringLiteral("else"),     QStringLiteral("for"),      QStringLiteral("goto"),
            QStringLiteral("if"),       QStringLiteral("return"),   QStringLiteral("switch"),
            QStringLiteral("throw"),    QStringLiteral("try"),      QStringLiteral("while"),
        },
        {
            QStringLiteral("alignas"),        QStringLiteral("alignof"),     QStringLiteral("asm"),
            QStringLiteral("auto"),           QStringLiteral("bool"),        QStringLiteral("char"),
            QStringLiteral("char8_t"),          QStringLiteral("char16_t"),  QStringLiteral("char32_t"),
            QStringLiteral("class"),          QStringLiteral("const"),     QStringLiteral("consteval"),
            QStringLiteral("constexpr"),      QStringLiteral("constinit"), QStringLiteral("const_cast"),
            QStringLiteral("decltype"),       QStringLiteral("delete"),    QStringLiteral("double"),
            QStringLiteral("dynamic_cast"),   QStringLiteral("enum"),      QStringLiteral("explicit"),
            QStringLiteral("export"),         QStringLiteral("extern"),  QStringLiteral("false"),
            QStringLiteral("float"),          QStringLiteral("friend"),  QStringLiteral("inline"),
            QStringLiteral("int"),            QStringLiteral("long"),    QStringLiteral("mutable"),
            QStringLiteral("namespace"),      QStringLiteral("new"),     QStringLiteral("noexcept"),
            QStringLiteral("nullptr"),        QStringLiteral("operator"), QStringLiteral("private"),
            QStringLiteral("protected"),      QStringLiteral("public"),  QStringLiteral("register"),
            QStringLiteral("reinterpret_cast"), QStringLiteral("requires"), QStringLiteral("short"),
            QStringLiteral("signed"),         QStringLiteral("sizeof"),  QStringLiteral("static"),
            QStringLiteral("static_assert"),  QStringLiteral("static_cast"), QStringLiteral("struct"),
            QStringLiteral("template"),       QStringLiteral("this"),    QStringLiteral("thread_local"),
            QStringLiteral("true"),           QStringLiteral("typedef"), QStringLiteral("typeid"),
            QStringLiteral("typename"),       QStringLiteral("union"),   QStringLiteral("unsigned"),
            QStringLiteral("using"),          QStringLiteral("virtual"), QStringLiteral("void"),
            QStringLiteral("volatile"),       QStringLiteral("wchar_t"),
        },
        true,
        true,
    };
    return definition;
}

const SyntaxDefinition &SyntaxDefinitionRegistry::definitionFor(SyntaxLanguage language)
{
    switch (language) {
    case SyntaxLanguage::C:
        return cDefinition();
    case SyntaxLanguage::Cpp:
        return cppDefinition();
    case SyntaxLanguage::None:
        break;
    }
    return cDefinition();
}

SyntaxLanguage SyntaxDefinitionRegistry::detectLanguage(const QString &filePath)
{
    return detectSyntaxLanguage(filePath);
}

} // namespace backend
