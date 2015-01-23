#include "ui_sc_editor.h"

#include <stdlib.h>
#include <stddef.h>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include "scintilla/include/Platform.h"
#include "scintilla/include/Scintilla.h"
#include "scintilla/include/ILexer.h"
#include "scintilla/include/SciLexer.h"
#include "scintilla/src/lexlib/LexerModule.h"
#include "scintilla/src/lexlib/PropSetSimple.h"
#include "scintilla/src/CharClassify.h"
#include "scintilla/src/SplitVector.h"
#include "scintilla/src/Partitioning.h"
#include "scintilla/src/RunStyles.h"
#include "scintilla/src/Catalogue.h"
#include "scintilla/src/ContractionState.h"
#include "scintilla/src/CellBuffer.h"
#include "scintilla/src/KeyMap.h"
#include "scintilla/src/Indicator.h"
#include "scintilla/src/XPM.h"
#include "scintilla/src/LineMarker.h"
#include "scintilla/src/Style.h"
#include "scintilla/src/ViewStyle.h"
#include "scintilla/src/Decoration.h"
#include "scintilla/src/CharClassify.h"
#include "scintilla/src/CaseFolder.h"
#include "scintilla/src/Document.h"
#include "scintilla/src/Selection.h"
#include "scintilla/src/PositionCache.h"
#include "scintilla/src/EditModel.h"
#include "scintilla/src/MarginView.h"
#include "scintilla/src/EditView.h"
#include "scintilla/src/Editor.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char glslKeyword[] =
"discard struct if else switch case default break goto return for while do continue";

const char glslType[] =
"attribute const in inout out uniform varying invariant "
"centroid flat smooth noperspective layout patch sample "
"subroutine lowp mediump highp precision "
"void float vec2 vec3 vec4 bvec2 bvec3 bvec4 ivec2 ivec3 ivec4 "
"uvec2 uvec3 uvec4 dvec2 dvec3 dvec4 "
"sampler1D sampler2D sampler3D isampler2D isampler1D isampler3D "
"usampler1D usampler2D usampler3D "
"sampler1DShadow sampler2DShadow sampler1DArray sampler2DArray "
"sampler1DArrayShadow sampler2DArrayShadow "
"samplerCube samperCubeShadow samperCubeArrayShadow ";

const char glslBuiltin[] =
"radians degrees sin cos tan asin acos atan sinh "
"cosh tanh asinh acosh atanh pow exp log exp2 "
"log2 sqrt inversesqrt abs sign floor trunc round "
"roundEven ceil fract mod modf min max clamp mix "
"step smoothstep isnan isinf floatBitsToInt floatBitsToUint "
"intBitsToFloat uintBitsToFloat fma frexp ldexp packUnorm2x16 "
"packUnorm4x8 packSnorm4x8 unpackUnorm2x16 unpackUnorm4x8 "
"unpackSnorm4x8 packDouble2x32 unpackDouble2x32 length distance "
"dot cross normalize ftransform faceforward reflect "
"refract matrixCompMult outerProduct transpose determinant "
"inverse lessThan lessThanEqual greaterThan greaterThanEqual "
"equal notEqual any all not uaddCarry usubBorrow "
"umulExtended imulExtended bitfieldExtract bitfildInsert "
"bitfieldReverse bitCount findLSB findMSB textureSize "
"textureQueryLOD texture textureProj textureLod textureOffset "
"texelFetch texelFetchOffset textureProjOffset textureLodOffset "
"textureProjLod textureProjLodOffset textureGrad textureGradOffset "
"textureProjGrad textureProjGradOffset textureGather "
"textureGatherOffset texture1D texture2D texture3D texture1DProj "
"texture2DProj texture3DProj texture1DLod texture2DLod "
"texture3DLod texture1DProjLod texture2DProjLod texture3DProjLod "
"textureCube textureCubeLod shadow1D shadow2D shadow1DProj "
"shadow2DProj shadow1DLod shadow2DLod shadow1DProjLod "
"shadow2DProjLod dFdx dFdy fwidth interpolateAtCentroid "
"interpolateAtSample interpolateAtOffset noise1 noise2 noise3 "
"noise4 EmitStreamVertex EndStreamPrimitive EmitVertex "
"EndPrimitive barrier "
"gl_VertexID gl_InstanceID gl_Position gl_PointSize "
"gl_ClipDistance gl_PrimitiveIDIn gl_InvocationID gl_PrimitiveID "
"gl_Layer gl_PatchVerticesIn gl_TessLevelOuter gl_TessLevelInner "
"gl_TessCoord gl_FragCoord gl_FrontFacing gl_PointCoord "
"gl_SampleID gl_SamplePosition gl_FragColor gl_FragData "
"gl_FragDepth gl_SampleMask gl_ClipVertex gl_FrontColor "
"gl_BackColor gl_FrontSecondaryColor gl_BackSecondaryColor "
"gl_TexCoord gl_FogFragCoord gl_Color gl_SecondaryColor "
"gl_Normal gl_Vertex gl_MultiTexCoord0 gl_MultiTexCoord1 "
"gl_MultiTexCoord2 gl_MultiTexCoord3 gl_MultiTexCoord4 "
"gl_MultiTexCoord5 gl_MultiTexCoord6 gl_MultiTexCoord7 gl_FogCoord "
"gl_MaxVertexAttribs gl_MaxVertexUniformComponents gl_MaxVaryingFloats "
"gl_MaxVaryingComponents gl_MaxVertexOutputComponents "
"gl_MaxGeometryInputComponents gl_MaxGeometryOutputComponents "
"gl_MaxFragmentInputComponents gl_MaxVertexTextureImageUnits "
"gl_MaxCombinedTextureImageUnits gl_MaxTextureImageUnits "
"gl_MaxFragmentUniformComponents gl_MaxDrawBuffers gl_MaxClipDistances "
"gl_MaxGeometryTextureImageUnits gl_MaxGeometryOutputVertices "
"gl_MaxGeometryTotalOutputComponents gl_MaxGeometryUniformComponents "
"gl_MaxGeometryVaryingComponents gl_MaxTessControlInputComponents "
"gl_MaxTessControlOutputComponents gl_MaxTessControlTextureImageUnits "
"gl_MaxTessControlUniformComponents "
"gl_MaxTessControlTotalOutputComponents "
"gl_MaxTessEvaluationInputComponents gl_MaxTessEvaluationOutputComponents "
"gl_MaxTessEvaluationTextureImageUnits "
"gl_MaxTessEvaluationUniformComponents gl_MaxTessPatchComponents "
"gl_MaxPatchVertices gl_MaxTessGenLevel gl_MaxTextureUnits "
"gl_MaxTextureCoords gl_MaxClipPlanes "
"gl_DepthRange gl_ModelViewMatrix gl_ProjectionMatrix "
"gl_ModelViewProjectionMatrix gl_TextureMatrix gl_NormalMatrix "
"gl_ModelViewMatrixInverse gl_ProjectionMatrixInverse "
"gl_ModelViewProjectionMatrixInverse gl_TextureMatrixInverse "
"gl_ModelViewMatrixTranspose gl_ProjectionMatrixTranspose "
"gl_ModelViewProjectionMatrixTranspose gl_TextureMatrixTranspose "
"gl_ModelViewMatrixInverseTranspose gl_ProjectionMatrixInverseTranspose "
"gl_ModelViewProjectionMatrixInverseTranspose "
"gl_TextureMatrixInverseTranspose gl_NormalScale gl_ClipPlane "
"gl_Point gl_FrontMaterial gl_BackMaterial gl_LightSource "
"gl_LightModel gl_FrontLightModelProduct gl_BackLightModelProduct "
"gl_FrontLightProduct gl_BackLightProduct gl_TextureEnvColor "
"gl_EyePlaneS gl_EyePlaneT gl_EyePlaneR gl_EyePlaneQ "
"gl_ObjectPlaneS gl_ObjectPlaneT gl_ObjectPlaneR gl_ObjectPlaneQ "
"gl_Fog";

class LexState : public LexInterface
{
    const LexerModule* m_lexCurrent;
    PropSetSimple m_propSet;
    int m_interfaceVersion;
    
public:
    int m_lexLanguage;
    
    explicit LexState(Document* pdoc_);
    virtual ~LexState();
    
    void SetLexer(uptr_t wParam);
    void SetLexerLanguage(const char* languageName);
    const char* DescribeWordListSets();
    void SetWordList(int n, const char* wl);
    const char* GetName() const;
    void* PrivateCall(int operation, void* pointer);
    const char* PropertyNames();
    int PropertyType(const char* name);
    const char* DescribeProperty(const char* name);
    void PropSet(const char* key, const char* val);
    const char* PropGet(const char* key) const;
    int PropGetInt(const char* key, int defaultValue = 0) const;
    int PropGetExpanded(const char* key, char* result) const;
    
    int LineEndTypesSupported();
    int AllocateSubStyles(int styleBase, int numberStyles);
    int SubStylesStart(int styleBase);
    int SubStylesLength(int styleBase);
    int StyleFromSubStyle(int subStyle);
    int PrimaryStyleFromStyle(int style);
    void FreeSubStyles();
    void SetIdentifiers(int style, const char* identifiers);
    int DistanceToSecondaryStyles();
    const char* GetSubStyleBases();
    
private:
    void SetLexerModule(const LexerModule* lex);
};

LexState::LexState(Document* pdoc_)
: LexInterface(pdoc_)
, m_lexCurrent(nullptr)
, m_interfaceVersion(lvOriginal)
, m_lexLanguage(SCLEX_CONTAINER)
{
}

LexState::~LexState()
{
    if (instance)
    {
        instance->Release();
        instance = nullptr;
    }
}

void LexState::SetLexerModule(const LexerModule* lex)
{
    if (lex != m_lexCurrent)
    {
        if (instance)
        {
            instance->Release();
            instance = nullptr;
        }
        
        m_interfaceVersion = lvOriginal;
        m_lexCurrent = lex;
        if (m_lexCurrent)
        {
            instance = m_lexCurrent->Create();
            m_interfaceVersion = instance->Version();
        }
        
        pdoc->LexerChanged();
    }
}

void LexState::SetLexer(uptr_t wParam)
{
    m_lexLanguage = int(wParam);
    if (m_lexLanguage == SCLEX_CONTAINER)
        SetLexerModule(0);
    else
    {
        const LexerModule* lex = Catalogue::Find(m_lexLanguage);
        if (!lex)
            lex = Catalogue::Find(SCLEX_NULL);
        
        SetLexerModule(lex);
    }
}

void LexState::SetLexerLanguage(const char* languageName)
{
    const LexerModule* lex = Catalogue::Find(languageName);
    if (!lex)
        lex = Catalogue::Find(SCLEX_NULL);
    
    if (lex)
        m_lexLanguage = lex->GetLanguage();
    
    SetLexerModule(lex);
}

const char* LexState::DescribeWordListSets()
{
    if (instance)
        return instance->DescribeWordListSets();
    else
        return 0;
}

void LexState::SetWordList(int n, const char* wl)
{
    if (instance)
    {
        int firstModification = instance->WordListSet(n, wl);
        if (firstModification >= 0)
            pdoc->ModifiedAt(firstModification);
    }
}

const char* LexState::GetName() const
{
    return m_lexCurrent ? m_lexCurrent->languageName : "";
}

void* LexState::PrivateCall(int operation, void *pointer)
{
    if (pdoc && instance)
        return instance->PrivateCall(operation, pointer);
    else
        return 0;
}

const char* LexState::PropertyNames()
{
    if (instance)
        return instance->PropertyNames();
    else
        return 0;
}

int LexState::PropertyType(const char* name)
{
    if (instance)
        return instance->PropertyType(name);
    else
        return SC_TYPE_BOOLEAN;
}

const char* LexState::DescribeProperty(const char* name)
{
    if (instance)
        return instance->DescribeProperty(name);
    else
        return 0;
}

void LexState::PropSet(const char* key, const char* val)
{
    m_propSet.Set(key, val);
    if (instance)
    {
        int firstModification = instance->PropertySet(key, val);
        if (firstModification >= 0)
            pdoc->ModifiedAt(firstModification);
    }
}

const char* LexState::PropGet(const char* key) const
{
    return m_propSet.Get(key);
}

int LexState::PropGetInt(const char* key, int defaultValue) const
{
    return m_propSet.GetInt(key, defaultValue);
}

int LexState::PropGetExpanded(const char* key, char* result) const
{
    return m_propSet.GetExpanded(key, result);
}

int LexState::LineEndTypesSupported()
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles *>(instance)->LineEndTypesSupported();

    return 0;
}

int LexState::AllocateSubStyles(int styleBase, int numberStyles)
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles *>(instance)->AllocateSubStyles(styleBase, numberStyles);

    return -1;
}

int LexState::SubStylesStart(int styleBase)
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles *>(instance)->SubStylesStart(styleBase);

    return -1;
}

int LexState::SubStylesLength(int styleBase)
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles*>(instance)->SubStylesLength(styleBase);

    return 0;
}

int LexState::StyleFromSubStyle(int subStyle)
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles*>(instance)->StyleFromSubStyle(subStyle);

    return 0;
}

int LexState::PrimaryStyleFromStyle(int style)
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles*>(instance)->PrimaryStyleFromStyle(style);

    return 0;
}

void LexState::FreeSubStyles()
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        static_cast<ILexerWithSubStyles*>(instance)->FreeSubStyles();
}

void LexState::SetIdentifiers(int style, const char* identifiers)
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        static_cast<ILexerWithSubStyles*>(instance)->SetIdentifiers(style, identifiers);
}

int LexState::DistanceToSecondaryStyles()
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles*>(instance)->DistanceToSecondaryStyles();

    return 0;
}

const char* LexState::GetSubStyleBases()
{
    if (instance && (m_interfaceVersion >= lvSubStyles))
        return static_cast<ILexerWithSubStyles*>(instance)->GetSubStyleBases();

    return "";
}

const size_t NB_FOLDER_STATE = 7;
const size_t FOLDER_TYPE = 0;
const uptr_t markersArray[][NB_FOLDER_STATE] =
{
    {SC_MARKNUM_FOLDEROPEN, SC_MARKNUM_FOLDER, SC_MARKNUM_FOLDERSUB, SC_MARKNUM_FOLDERTAIL, SC_MARKNUM_FOLDEREND,        SC_MARKNUM_FOLDEROPENMID,     SC_MARKNUM_FOLDERMIDTAIL},
    {SC_MARK_MINUS,         SC_MARK_PLUS,      SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
    {SC_MARK_ARROWDOWN,     SC_MARK_ARROW,     SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
    {SC_MARK_CIRCLEMINUS,   SC_MARK_CIRCLEPLUS,SC_MARK_VLINE,        SC_MARK_LCORNERCURVE,  SC_MARK_CIRCLEPLUSCONNECTED, SC_MARK_CIRCLEMINUSCONNECTED, SC_MARK_TCORNERCURVE},
    {SC_MARK_BOXMINUS,      SC_MARK_BOXPLUS,   SC_MARK_VLINE,        SC_MARK_LCORNER,       SC_MARK_BOXPLUSCONNECTED,    SC_MARK_BOXMINUSCONNECTED,    SC_MARK_TCORNER}
};

struct ScEditor : public Editor
{
private:
    int m_width;
    int m_height;
    
public:
    
    virtual ~ScEditor()
    {
    }
    
    void Update()
    {
        Tick();
    }
    
    void Render()
    {
        PRectangle rcPaint = GetClientRectangle();
        
        AutoSurface surfaceWindow(this);
        if (surfaceWindow)
        {
            Paint(surfaceWindow, rcPaint);
            surfaceWindow->Release();
        }
    }
    
    void SetAStyle(int style, ColourDesired fore, ColourDesired back=0xFFFFFFFF, int size=-1, const char *face=0)
    {
        SendCommand(SCI_STYLESETFORE, uptr_t(style), fore.AsLong());
        SendCommand(SCI_STYLESETBACK, uptr_t(style), back.AsLong());
        if (size >= 1)
            SendCommand(SCI_STYLESETSIZE, uptr_t(style), size);
        if (face)
            SendCommand(SCI_STYLESETFONT, uptr_t(style), reinterpret_cast<sptr_t>(face));
    }
    
	void Initialise()
	{
        wMain = WindowID(1);
        wMargin = WindowID(2);
        
        // We need to disable buffered draw so Scintilla doesn't keep a yoffset of 0
        // when rendering text, thinking we are blitting through a pixmap. We want a
        // single draw list for efficiency.
        view.bufferedDraw = false;
        
        SendCommand(SCI_SETLEXER, SCLEX_CPP);
        SendCommand(SCI_SETSTYLEBITS, 7);
        
        SendCommand(SCI_SETKEYWORDS, 0,
                   reinterpret_cast<sptr_t>(glslKeyword));
        SendCommand(SCI_SETKEYWORDS, 1,
                   reinterpret_cast<sptr_t>(glslType));
        SendCommand(SCI_SETKEYWORDS, 2,
                   reinterpret_cast<sptr_t>(glslBuiltin));
        SendCommand(SCI_SETSTYLEBITS, 7);
        
        SendCommand(SCI_SETPROPERTY, reinterpret_cast<uptr_t>("fold"), reinterpret_cast<sptr_t>("1"));
        
        int fontSize = 24;
        
        const char* fontName = "data/font/source_code_pro/SourceCodePro-Medium.ttf";
        
        // Set up the global default style. These attributes are used wherever no explicit choices are made.
        SetAStyle(STYLE_DEFAULT,     0xFFFFFFFF, 0xD0000000, fontSize, fontName);
        SendCommand(SCI_STYLECLEARALL);	// Copies global style to all others
        SetAStyle(STYLE_INDENTGUIDE, 0xFFC0C0C0, 0xD0000000, fontSize, fontName);
        SetAStyle(STYLE_BRACELIGHT,  0xFF00FF00, 0xD0000000, fontSize, fontName);
        SetAStyle(STYLE_BRACEBAD,    0xFF0000FF, 0xD0000000, fontSize, fontName);
        SetAStyle(STYLE_LINENUMBER,  0xFFC0C0C0, 0xD0333333, fontSize, fontName);
        SendCommand(SCI_SETFOLDMARGINCOLOUR,   1, 0xD01A1A1A);
        SendCommand(SCI_SETFOLDMARGINHICOLOUR, 1, 0xD01A1A1A);
        SendCommand(SCI_SETSELBACK,            1, 0xD0CC9966);
        SendCommand(SCI_SETCARETFORE,          0xFFFFFFFF, 0);
        SendCommand(SCI_SETCARETLINEVISIBLE,   1);
        SendCommand(SCI_SETCARETLINEBACK,      0xFFFFFFFF);
        SendCommand(SCI_SETCARETLINEBACKALPHA, 0x20);
        
        SendCommand(SCI_SETMARGINWIDTHN, 0, 44);//Calculate correct width
        SendCommand(SCI_SETMARGINWIDTHN, 1, 20);//Calculate correct width
        SendCommand(SCI_SETMARGINMASKN, 1, SC_MASK_FOLDERS);//Calculate correct width
        
        for (unsigned int i = 0 ; i < NB_FOLDER_STATE ; i++)
        {
            SendCommand(SCI_MARKERDEFINE, markersArray[FOLDER_TYPE][i], sptr_t(markersArray[4][i]));
            SendCommand(SCI_MARKERSETBACK, markersArray[FOLDER_TYPE][i], 0xFF6A6A6A);
            SendCommand(SCI_MARKERSETFORE, markersArray[FOLDER_TYPE][i], 0xFF333333);
        }
        
        SendCommand(SCI_SETUSETABS, 1);
        SendCommand(SCI_SETTABWIDTH, 4);
        SendCommand(SCI_SETINDENTATIONGUIDES, SC_IV_REAL);
        
        SetAStyle(SCE_C_DEFAULT,      0xFFFFFFFF, 0xD0000000, fontSize, fontName);
        SetAStyle(SCE_C_WORD,         0xFF0066FF, 0xD0000000);
        SetAStyle(SCE_C_WORD2,        0xFFFFFF00, 0xD0000000);
        //WTF??? SetAStyle(SCE_C_GLOBALCLASS, 0xFF0000FF, 0xFF000000);
        SetAStyle(SCE_C_PREPROCESSOR, 0xFFC0C0C0, 0xD0000000);
        SetAStyle(SCE_C_NUMBER,       0xFF0080FF, 0xD0000000);
        SetAStyle(SCE_C_OPERATOR,     0xFF00CCFF, 0xD0000000);
        SetAStyle(SCE_C_COMMENT,      0xFF00FF00, 0xD0000000);
        SetAStyle(SCE_C_COMMENTLINE,  0xFF00FF00, 0xD0000000);
        
        const char* text = "precision highp float;\n"
        "\n"
        "varying vec3 n;\n"
        "varying vec2 uv;\n"
        "\n"
        "uniform sampler2D tex;\n"
        "\n"
        "#pragma include \"noise2D.glsl\" // for snoise(vec2 v)\n"
        "#pragma include \"noise3D.glsl\" //  for snoise(vec3 v)\n"
        "#pragma include \"noise4D.glsl\" //  for snoise(vec4 v)\n"
        "#pragma include \"cellular2D.glsl\" //  for cellular(vec2 P)\n"
        "#pragma include \"cellular2x2.glsl\" //  for cellular2x2(vec2 P)\n"
        "#pragma include \"cellular2x2x2.glsl\" //  for cellular2x2x2(vec3 P)\n"
        "#pragma include \"cellular3D.glsl\" //  cellular(vec3 P)\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "\tvec3 eyeSpaceLightDirection = vec3(0.0,0.0,1.0);\n"
        "\tfloat diffuse = max(0.0,dot(normalize(n),eyeSpaceLightDirection));\n"
        "\tgl_FragColor = vec4(texture2D(tex,uv).xyz*diffuse,1.0);\n"
        "}";

        
        SendCommand(SCI_ADDTEXT, strlen(text),
                   reinterpret_cast<sptr_t>(static_cast<const char *>(text)));
	}
    
    void Resize(int width, int height)
    {
        m_width  = width;
        m_height = height;
        
        // GW-TODO: Likely need to adjust a member var on wMain and make
        // GetClientRectangle return that value.
        
        //float w1 = m_width  - 80.0f;
        //float h1 = m_height - 80.0f;/*80=30+20+30*/
        
        //SetSize(w1 * 0.7f, h1 * 0.7f);
    }

	void SetVerticalScrollPos()
	{
	}

	void SetHorizontalScrollPos()
	{
	}

	bool ModifyScrollBars(int nMax, int nPage)
	{
		(void)nMax;
		(void)nPage;
		return false;
	}

	void ClaimSelection()
	{
	}

	void Copy()
	{
	}

	void Paste()
	{
	}

	void NotifyChange()
	{
	}

	void NotifyParent(SCNotification scn)
	{
		(void)scn;
	}

	void CopyToClipboard(const SelectionText& selectedText)
	{
		(void)selectedText;
	}

	void SetMouseCapture(bool on)
	{
		(void)on;
	}

	bool HaveMouseCapture()
	{
		return false;
	}

	sptr_t DefWndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam)
	{
        switch (iMessage)
        {
            default:
                break;

            case SCI_SETLEXER:
                DocumentLexState()->SetLexer(static_cast<uptr_t>(wParam));
                break;
                
            case SCI_GETLEXER:
                return DocumentLexState()->m_lexLanguage;
                
            case SCI_COLOURISE:
                if (DocumentLexState()->m_lexLanguage == SCLEX_CONTAINER)
                {
                    pdoc->ModifiedAt(static_cast<int>(wParam));
                    NotifyStyleToNeeded((lParam == -1) ? pdoc->Length() : static_cast<int>(lParam));
                }
                else
                    DocumentLexState()->Colourise(static_cast<int>(wParam), static_cast<int>(lParam));

                Redraw();
                break;
                
            case SCI_SETPROPERTY:
                DocumentLexState()->PropSet(reinterpret_cast<const char*>(wParam),
                                            reinterpret_cast<const char*>(lParam));
                break;
                
            case SCI_GETPROPERTY:
                return StringResult(lParam, DocumentLexState()->PropGet(reinterpret_cast<const char*>(wParam)));
                
            case SCI_GETPROPERTYEXPANDED:
                return DocumentLexState()->PropGetExpanded(reinterpret_cast<const char*>(wParam),
                                                           reinterpret_cast<char*>(lParam));
                
            case SCI_GETPROPERTYINT:
                return DocumentLexState()->PropGetInt(reinterpret_cast<const char*>(wParam), static_cast<int>(lParam));
                
            case SCI_SETKEYWORDS:
                DocumentLexState()->SetWordList(int(wParam), reinterpret_cast<const char*>(lParam));
                break;
                
            case SCI_SETLEXERLANGUAGE:
                DocumentLexState()->SetLexerLanguage(reinterpret_cast<const char*>(lParam));
                break;
                
            case SCI_GETLEXERLANGUAGE:
                return StringResult(lParam, DocumentLexState()->GetName());
                
            case SCI_PRIVATELEXERCALL:
                return reinterpret_cast<sptr_t>(DocumentLexState()->PrivateCall(int(wParam), reinterpret_cast<void*>(lParam)));
                
            case SCI_GETSTYLEBITSNEEDED:
                return 8;
                
            case SCI_PROPERTYNAMES:
                return StringResult(lParam, DocumentLexState()->PropertyNames());
                
            case SCI_PROPERTYTYPE:
                return DocumentLexState()->PropertyType(reinterpret_cast<const char*>(wParam));
                
            case SCI_DESCRIBEPROPERTY:
                return StringResult(lParam, DocumentLexState()->DescribeProperty(reinterpret_cast<const char*>(wParam)));
                
            case SCI_DESCRIBEKEYWORDSETS:
                return StringResult(lParam, DocumentLexState()->DescribeWordListSets());
                
            case SCI_GETLINEENDTYPESSUPPORTED:
                return DocumentLexState()->LineEndTypesSupported();
                
            case SCI_ALLOCATESUBSTYLES:
                return DocumentLexState()->AllocateSubStyles(int(wParam), int(lParam));
                
            case SCI_GETSUBSTYLESSTART:
                return DocumentLexState()->SubStylesStart(int(wParam));
                
            case SCI_GETSUBSTYLESLENGTH:
                return DocumentLexState()->SubStylesLength(int(wParam));
                
            case SCI_GETSTYLEFROMSUBSTYLE:
                return DocumentLexState()->StyleFromSubStyle(int(wParam));
                
            case SCI_GETPRIMARYSTYLEFROMSTYLE:
                return DocumentLexState()->PrimaryStyleFromStyle(int(wParam));
                
            case SCI_FREESUBSTYLES:
                DocumentLexState()->FreeSubStyles();
                break;
                
            case SCI_SETIDENTIFIERS:
                DocumentLexState()->SetIdentifiers(int(wParam), reinterpret_cast<const char*>(lParam));
                break;
                
            case SCI_DISTANCETOSECONDARYSTYLES:
                return DocumentLexState()->DistanceToSecondaryStyles();
                
            case SCI_GETSUBSTYLEBASES:
                return StringResult(lParam, DocumentLexState()->GetSubStyleBases());
        }
        
        // GW: These are commands\events not handled by Scintilla Editor
        // Do not call into WndProc or it'll be recursive overflow.
        return 0;//WndProc(iMessage, wParam, lParam);
	}
    
    sptr_t SendCommand(unsigned int iMessage, uptr_t wParam = 0, sptr_t lParam = 0)
    {
        return WndProc(iMessage, wParam, lParam);
    }
    
    LexState* DocumentLexState()
    {
        if (!pdoc->pli)
            pdoc->pli = new LexState(pdoc);

        return static_cast<LexState *>(pdoc->pli);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ScEditor* ScEditor_create(int width, int height)
{
	ScEditor* ed = new ScEditor;
    
    ed->Initialise();
    ScEditor_resize(ed, width, height);

	return ed;
}

void ScEditor_resize(ScEditor* editor, int width, int height)
{
    if (editor)
        editor->Resize(width, height);
}

void ScEditor_tick(ScEditor* editor)
{
    if (editor)
        editor->Update();
}

void ScEditor_render(ScEditor* editor)
{
    if (editor)
    {
        //editor->BraceMatch();
        editor->Render();
    }
}