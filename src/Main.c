// #include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
// #include "C:/Wichtig/System/Static/Library/Victor2.h"
// #include "C:/Wichtig/System/Static/Library/Vdctor2.h"
// #include "C:/Wichtig/System/Static/Library/Complex.h"
// #include "C:/Wichtig/System/Static/Library/TransformedView.h"
// #include "C:\Wichtig\System\Static\Container\Vector.h"

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "DebugWindow.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Container/Vector.h"


TransformedView tv;
Rect rect;
Vec2 v;
Vec2 a;

DebugWindow dg_w;

void EventHandler(void* r,EventId* e){
    Renderable* ra = (Renderable*)r;
    Component* c = Component_Scene_FindRR(&dg_w.cg,ra);
	
    if(c){
        char* name = Component_Attr_is(c,"name") ? (char*)Component_Attr_Get(c,"name") : NULL;
		if(name==NULL){
			printf("[Main]: EventHandler -> name Error!\n");
		}else if(CStr_Cmp(name,"px")){
			Slider* s = (Slider*)r;
			rect.p.x = s->scrolled;
		}else if(CStr_Cmp(name,"py")){
			Slider* s = (Slider*)r;
			rect.p.y = s->scrolled;
		}else if(CStr_Cmp(name,"dx")){
			Slider* s = (Slider*)r;
			rect.d.x = s->scrolled;
		}else if(CStr_Cmp(name,"dy")){
			Slider* s = (Slider*)r;
			rect.d.y = s->scrolled;
		}else if(CStr_Cmp(name,"vx")){
			Slider* s = (Slider*)r;
			v.x = s->scrolled - 0.5f;
		}else if(CStr_Cmp(name,"vy")){
			Slider* s = (Slider*)r;
			v.y = s->scrolled - 0.5f;
		}else if(CStr_Cmp(name,"ax")){
			Slider* s = (Slider*)r;
			a.x = s->scrolled - 0.5f;
		}else if(CStr_Cmp(name,"ay")){
			Slider* s = (Slider*)r;
			a.y = s->scrolled - 0.5f;
		}
    }
}

void Setup(AlxWindow* w){
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	TransformedView_Offset(&tv,(Vec2){ 0.0f,0.0f });

	rect = (Rect){ 0.0f,0.0f,0.05f,0.05f };
	v = (Vec2){ 0.0f,0.0f };
	a = (Vec2){ 0.0f,0.0f };

	ResizeAlxFont(16,16);

	dg_w = DebugAlxWindow_New("./code/Main.alxml",EventHandler);
	DebugAlxWindow_Start(&dg_w);
}
void Update(AlxWindow* w){
	tv.ZoomSpeed = (float)w->ElapsedTime;
	TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });

	v = Vec2_Add(v,Vec2_Mulf(a,w->ElapsedTime));
	rect.p = Vec2_Add(rect.p,Vec2_Mulf(v,w->ElapsedTime));

	if(rect.p.x<0.0f){
		rect.p.x = 0.0f;
		v.x *= -1.0f;
	}
	if(rect.p.y<0.0f){
		rect.p.y = 0.0f;
		v.y *= -1.0f;
	}
	if(rect.p.x>1.0f - rect.d.x){
		rect.p.x = 1.0f - rect.d.x;
		v.x *= -1.0f;
	}
	if(rect.p.y>1.0f - rect.d.y){
		rect.p.y = 1.0f - rect.d.y;
		v.y *= -1.0f;
	}

	Clear(BLACK);

	Vec2 bg_p = TransformedView_WorldScreenPos(&tv,(Vec2){ 0.0f,0.0f });
	Vec2 bg_d = TransformedView_WorldScreenLength(&tv,(Vec2){ 1.0f,1.0f });
	RenderRect(bg_p.x,bg_p.y,bg_d.x,bg_d.y,BLUE);

	Vec2 p = TransformedView_WorldScreenPos(&tv,rect.p);
	Vec2 d = TransformedView_WorldScreenLength(&tv,rect.d);
	RenderRect(p.x,p.y,d.x,d.y,RED);


	String str = String_Format("| Offset: %f,%f - Zoom: %f,%f |",tv.Offset.x,tv.Offset.y,tv.Scale.x,tv.Scale.y);
	RenderCStrSize(str.Memory,str.size,0,0,WHITE);
	String_Free(&str);
}
void Delete(AlxWindow* w){
    DebugAlxWindow_Free(&dg_w);
}

int main(){
    if(Create("Debug Window",1200,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}