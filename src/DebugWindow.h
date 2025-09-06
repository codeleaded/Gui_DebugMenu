#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/ComponentPack.h"
#include "/home/codeleaded/System/Static/Library/ComponentML.h"
#include "/home/codeleaded/System/Static/Library/ComponentDefines.h"

typedef unsigned char DebugInteracterType;

typedef struct DebugInteracter {
    DebugInteracterType id;
    unsigned int size;
    void* data;
    char* name;
    char* (*tostr)(void*,unsigned int);
} DebugInteracter;

DebugInteracter DebugInteracter_New(DebugInteracterType id,unsigned int size,void* data,char* name,char* (*tostr)(void*,unsigned int)){
    DebugInteracter di;
    di.id = id;
    di.size = size;
    
    di.data = malloc(size);
    memcpy(di.data,data,size);

    di.name = CStr_Cpy(name);
    di.tostr = tostr;
    return di;
}
void DebugInteracter_Free(DebugInteracter* di){
    CStr_Free(&di->name);
    
    if(di->data) free(di->data);
    di->data = NULL;
}


typedef struct DebugWindow {
    AlxWindow w;
    Thread t;
    Scene scene;
    ComponentPack cg;
    ComponentML cml;
} DebugWindow;

void DebugAlxWindow_Setup(DebugWindow* dw){

}
void DebugAlxWindow_Update(DebugWindow* dw,double w->ElapsedTime){
    Scene_Update(&dw->scene,dw->w.Strokes,(Vec2){ dw->w.MouseX,dw->w.MouseY },(Vec2){ dw->w.MouseBeforeX,dw->w.MouseBeforeY });
	Graphics_Clear(dw->w.Buffer,dw->w.Width,dw->w.Height,BLACK);
	Scene_Render(dw->w.Buffer,dw->w.Width,dw->w.Height,&dw->scene);
}
void DebugAlxWindow_Delete(DebugWindow* dw){

}

void* DebugAlxWindow_Executer(void* a){
    DebugWindow* dw = (DebugWindow*)a;
    AlxAlxWindow_Start(&dw->w);
}

DebugWindow DebugAlxWindow_New(char* File_alxml,void (*DebugAlxWindow_EventHandler)(void*,EventId*)){
    DebugWindow dw;
    
    dw.scene = Scene_New();

	dw.cml = ComponentML_New(
        KeywordMap_Make((KeywordRP[]){
            KEYWORD_END
        }),
        OperatorMap_Make((OperatorRP[]){
            OperatorRP_Make((TT_Type[]){ TOKEN_EQUAL_SIGN,TOKEN_END },                          TOKEN_CUSTOM_ASS,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_PLUS_SIGN,TOKEN_END },                           TOKEN_CUSTOM_ADD,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_END },                          TOKEN_CUSTOM_SUB,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ASTERISK,TOKEN_END },                            TOKEN_CUSTOM_MUL,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_SLASH,TOKEN_END },                               TOKEN_CUSTOM_DIV,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,TOKEN_END },         TOKEN_CUSTOM_DEC,ARGS_IGNORE),
            
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_END },                    TOKEN_CUSTOM_TAGL,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_R,TOKEN_END },                    TOKEN_CUSTOM_TAGR,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_SLASH,TOKEN_END },        TOKEN_CUSTOM_TAGC,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_EXCLAMATION_MARK,
                                         TOKEN_END },                                           TOKEN_CUSTOM_TAGLEX0,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_EXCLAMATION_MARK,
                                         TOKEN_MINUS_SIGN,TOKEN_END },                          TOKEN_CUSTOM_TAGLEX1,ARGS_IGNORE),
    
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,TOKEN_END },         TOKEN_CUSTOM_LINECOMMENT,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_EXCLAMATION_MARK,
                                         TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,TOKEN_END },         TOKEN_CUSTOM_BLOCKCOMMENTSTART,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,
                                         TOKEN_ANGLE_BRACKETS_R,TOKEN_END },                    TOKEN_CUSTOM_BLOCKCOMMENTEND,ARGS_IGNORE),
            OperatorRP_End()
        }),
        DirectiveMap_Make((Directive[]){
            Directive_Comment_Block(TOKEN_CUSTOM_BLOCKCOMMENTSTART,TOKEN_CUSTOM_BLOCKCOMMENTEND,True),
            Directive_Compress_Tag(TOKEN_CUSTOM_TAGOPEN,TOKEN_CUSTOM_TAGL,TOKEN_CUSTOM_TAGR),
            Directive_Compress_Tag(TOKEN_CUSTOM_TAGCLOSE,TOKEN_CUSTOM_TAGC,TOKEN_CUSTOM_TAGR),
            Directive_End()
        }),
        TT_TypeMap_Make((TT_Type[]){
            TOKEN_NEWLINE,
            TOKEN_END
        }),
        ReseterMap_Make((Reseter[]){ 
            // operators who might have two versions: 1 arg vs. 2 args are not listed: -
            { TOKEN_NONE,RESETER_NONE },
            { TOKEN_NUMBER,RESETER_ADD1 },
            { TOKEN_FLOAT,RESETER_ADD1 },
            { TOKEN_STRING,RESETER_ADD1 },
            
            { TOKEN_SPACE,RESETER_NONE },
            { TOKEN_NEWLINE,RESETER_NONE },// without semicolon like python
            { TOKEN_CARTURN,RESETER_NONE },
            { TOKEN_HTAB,RESETER_NONE },
            { TOKEN_VTAB,RESETER_NONE },
    
            { TOKEN_COMMA,RESETER_RST },

            { TOKEN_PARENTHESES_R,RESETER_TO1 },
            { TOKEN_SQUARE_BRACKETS_R,RESETER_NONE },
            { TOKEN_CURLY_BRACES_R,RESETER_NONE },
            //{ TOKEN_ANGLE_BRACKETS_R,RESETER_NONE }, //also < and > operator
            { TOKEN_DOUBLE_ANGLE_BRACKETS_R,RESETER_NONE },
            { TOKEN_WHITE_SQUARE_BRACKETS_R,RESETER_NONE },
            
            { TOKEN_PARENTHESES_L,RESETER_RST },
            { TOKEN_SQUARE_BRACKETS_L,RESETER_RST },
            { TOKEN_CURLY_BRACES_L,RESETER_RST },
            //{ TOKEN_ANGLE_BRACKETS_L,RESETER_RST }, //also < and > operator
            { TOKEN_DOUBLE_ANGLE_BRACKETS_L,RESETER_RST },
            { TOKEN_WHITE_SQUARE_BRACKETS_L,RESETER_RST },
            { TOKEN_SEMICOLON,RESETER_RST },
            { TOKEN_COLON,RESETER_RST },
            { TOKEN_EXCLAMATION_MARK,RESETER_RST },
            { TOKEN_QUESTION_MARK,RESETER_RST },
            { TOKEN_ELLIPSIS,RESETER_RST },
            { TOKEN_PLUS_SIGN,RESETER_RST },
            { TOKEN_MULTIPLICATION_SIGN,RESETER_RST },
            { TOKEN_DIVISION_SIGN,RESETER_RST },
            { TOKEN_EQUAL_SIGN,RESETER_RST },
            { TOKEN_NOT_EQUAL_SIGN,RESETER_RST },
            { TOKEN_LESS_THAN,RESETER_RST },
            { TOKEN_GREATER_THAN,RESETER_RST },
            { TOKEN_LESS_THAN_OR_EQUAL_TO,RESETER_RST },
            { TOKEN_GREATER_THAN_OR_EQUAL_TO,RESETER_RST },
            { TOKEN_APPROXIMATION_SIGN,RESETER_RST },
            { TOKEN_PLUS_MINUS_SIGN,RESETER_RST },
            { TOKEN_AMPERSAND,RESETER_RST },
            { TOKEN_AT_SIGN,RESETER_RST },
            { TOKEN_ASTERISK,RESETER_RST },
            { TOKEN_CARET,RESETER_RST },
            { TOKEN_TILDE,RESETER_RST },
            { TOKEN_SLASH,RESETER_RST },
            { TOKEN_BACKSLASH,RESETER_RST },
            { TOKEN_UNDERSCORE,RESETER_RST },
            { TOKEN_PIPE,RESETER_RST },
            { TOKEN_PERCENT,RESETER_RST },
            { TOKEN_HASH_POUND_SIGN,RESETER_RST },
            { TOKEN_SINGLE_QUOTE,RESETER_RST },
            { TOKEN_DOUBLE_QUOTE,RESETER_RST },
            { TOKEN_BACKTICK,RESETER_RST },
            { TOKEN_PRIME,RESETER_RST },
            { TOKEN_DOUBLE_PRIME,RESETER_RST },
            { TOKEN_SECTION_SYMBOL,RESETER_RST },
            { TOKEN_DEGREE_SYMBOL,RESETER_RST },
            { TOKEN_FRACTION_SLASH,RESETER_RST },
            { TOKEN_INTERROBANG,RESETER_RST },
            { TOKEN_BULLET_POINT,RESETER_RST },
    
            { TOKEN_CAST,RESETER_ADD1 },
            { TOKEN_FUNCTION,RESETER_ADD1 },
            { TOKEN_CONSTSTRING_SINGLE,RESETER_ADD1 },
            { TOKEN_CONSTSTRING_DOUBLE,RESETER_ADD1 },

            { TOKEN_END,RESETER_RST }
        })
    );
    
    ComponentML_Source(&dw.cml,"./code/Main.alxml");
    //TokenMap_Print(&cml.tm);

    dw.cg = ComponentML_Build(&dw.cml);
    Component_Scene_Push_R(&dw.cg,&dw.scene);
    Component_Scene_Event_R(&dw.cg,&dw.scene,DebugAlxWindow_EventHandler);

    Component* c_window = (Component*)Vector_Get(&dw.cg.childs,0);
    char* title = Component_Attr_is(c_window,"title") ? (char*)Component_Attr_Get(c_window,"title") : NULL;
    int Width = Component_Attr_is(c_window,"width") ? Number_Parse(Component_Attr_Get(c_window,"width")) : 100;
    int Height = Component_Attr_is(c_window,"height") ? Number_Parse(Component_Attr_Get(c_window,"height")) : 100;
    int pw = Component_Attr_is(c_window,"pw") ? Number_Parse(Component_Attr_Get(c_window,"pw")) : 1;
    int ph = Component_Attr_is(c_window,"ph") ? Number_Parse(Component_Attr_Get(c_window,"ph")) : 1;
	
    dw.w = AlxAlxWindow_New(
        title,
        Width,
        Height,
        pw,
        ph,
        (void*)DebugAlxWindow_Setup,
        (void*)DebugAlxWindow_Update,
        (void*)DebugAlxWindow_Delete
    );

    if(!dw.w.Running){
        printf("[DebugWindow]: New -> init failed because AlxWindow Create did!\n");
        return dw;
    }

    dw.t = Thread_Null();
    return dw;
}
void DebugAlxWindow_Start(DebugWindow* dw){
    if(!dw->t.running){
        dw->t = Thread_New(NULL,DebugAlxWindow_Executer,dw);
        Thread_Start(&dw->t);
    }
}
void DebugAlxWindow_Free(DebugWindow* dw){
    dw->w.Running = 0;
    dw->t.running = 0;

    Thread_Join(&dw->t,NULL);

    Scene_Free(&dw->scene);
    Component_Free(&dw->cg);
    ComponentML_Free(&dw->cml);
}

#endif