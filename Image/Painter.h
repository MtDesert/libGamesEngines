#ifndef PAINTER_H
#define PAINTER_H

#include"Pixmap.h"
#include"Triangle.h"
#include"Rectangle.h"

template<typename ColorType>
class Painter{
public:
	Painter(Pixmap<ColorType> &pixmap):pixmap(pixmap){}
	void drawArc();
	void drawChord();
	void drawConvexPolygon();
	void drawPie();
	void drawRoundedRect();
	//单条直线
	void drawLine(const Point2D<int> &p1,const Point2D<int> &p2){
		//printf("drawLine: %s--%s\n",p1.toString().data(),p2.toString().data());
		if(p1==p2){//点
			pixmap.setColor(p1,color);
			return;
		}
		//向量
		Point2D<int> vec=p2-p1;
		//长度
		Point2D<int> len(abs(vec.x()),abs(vec.y()));
		//微积分
		Point2D<int> delta((len.x()?vec.x()/len.x():0),(len.y()?vec.y()/len.y():0));
		//特殊情况(提高执行效率,根据情况可注释掉)
		if(vec.x()==0){//竖线
			int y=p1.y();
			for(;y!=p2.y();y+=delta.y())pixmap.setColor(p1.x(),y,color);
			pixmap.setColor(p1.x(),y,color);
			return;
		}else if(vec.y()==0){//横线
			int x=p1.x();
			for(;x!=p2.x();x+=delta.x())pixmap.setColor(x,p1.y(),color);
			pixmap.setColor(x,p1.y(),color);
			return;
		}else if(len.x()==len.y()){//平分xy轴的斜线
			auto p=p1;
			for(int i=0;i<=len.x();++i){
				pixmap.setColor(p,color);
				p+=delta;
			}
			return;
		}
		//判断特征(lng表示较长的部分,sht表示较短的部分)
		Point2D<int> p=p1;//p代表当前点，用来进行迭代运算
		const int *lngP1,*shtP1,*lngLen,*shtLen,*lngDelta,*shtDelta;
		int *lngP,*shtP;
		//确定长短
		bool b=(len.x()>=len.y());
		lngP=&(b?p.x():p.y());
		shtP=&(b?p.y():p.x());
		lngP1=&(b?p1.x():p1.y());
		shtP1=&(b?p1.y():p1.x());
		lngLen=&(b?len.x():len.y());
		shtLen=&(b?len.y():len.x());
		lngDelta=&(b?delta.x():delta.y());
		shtDelta=&(b?delta.y():delta.x());
		//开始画线
		int product,quotient,remainder;
		for(int i=0;i<=(*lngLen);++i){
			//确定位置
			*lngP=(*lngP1)+(*lngDelta)*i;
			product=(*shtLen)*i;
			quotient=product/(*lngLen);
			remainder=product%(*lngLen);
			if(remainder*2>(*lngLen))++quotient;
			*shtP=(*shtP1)+(*shtDelta)*quotient;
			//绘制点
			pixmap.setColor(p,color);
		}
	}
	void drawLine(const Line<int,2> &line)const{
		drawLine(line.p0,line.p1);
	}
	//折线
	void drawBrokenLine(const list<Point2D<int> > &points){
		auto current=points.begin();
		if(current==points.end())return;
		auto next=current;
		++next;
		drawLine(*current,*current);
		while(next!=points.end()){
			drawLine(*current,*next);
			current=next;
			++next;
		}
	}
	//多边形
	void drawPolygon(const list<Point2D<int> > &points){
		drawBrokenLine(points);//先画折线
		auto first=points.begin();
		auto last=points.rbegin();
		if(first==points.end()||last==points.rend())return;
		drawLine(*last,*first);//最后再画一条线
	}
	//三角形
	void drawTriangle(const Triangle2D<int> &triangle){
		drawLine(triangle.line0());
		drawLine(triangle.line1());
		drawLine(triangle.line2());
	}
	//矩形
	void drawRect(const Rectangle2D<int> &rect){
		auto bl(rect.bottomLeft());
		auto br(rect.bottomRight());
		auto tl(rect.topLeft());
		auto tr(rect.topRight());
		drawLine(bl,br);
		drawLine(br,tr);
		drawLine(tr,tl);
		drawLine(tl,bl);
	}
	//画圆辅助函数
	void drawCircleXY(int xc,int yc,int x,int y){
		pixmap.setColor(xc+x,yc+y,color);
		pixmap.setColor(xc-x,yc+y,color);
		pixmap.setColor(xc+x,yc-y,color);
		pixmap.setColor(xc-x,yc-y,color);
	}
	//画圆(圆心,半径)
	void drawCircle(const Point2D<int> &center,int r){
		Point2D<int> p(0,r);
		int upLimit=r*r+r;//因为(r+1)^2 - r^2 == r*2+1,取此值的一半即r+0.5,则可得出r和r+1为4舍5入的判定依据
		while(p.x()<=p.y()){//画1/8的圆弧
			drawCircleXY(center.x(),center.y(),p.x(),p.y());
			drawCircleXY(center.x(),center.y(),p.y(),p.x());
			//寻找下一个坐标
			++p.x();
			if(p.distance2()>upLimit){
				--p.y();
			}
		}
	}
	//画圆(矩形区域的内接椭圆)
	void drawEllipse(const Rectangle2D<int> &rect){
		//向量
		decltype(rect.p0) vec(rect.p1-rect.p0);
		if(vec.x()==0||vec.y()==0){
			drawLine(rect.p0,rect.p1);
			return;
		}
		decltype(vec) diameter(abs(vec.x()),abs(vec.y()));//直径
		decltype(vec) radius(diameter.x()/2,diameter.y()/2);//半径
		decltype(vec) c1=rect.center(),c2=c1,c3=c1,c4=c1;//圆心,有可能在4个象限上
		//确定X坐标
		if(diameter.x()%2){
			if(vec.x()>0){++c1.x();++c4.x();}
			if(vec.x()<0){--c2.x();--c3.x();}
		}
		//确定Y坐标
		if(diameter.y()%2){
			if(vec.y()>0){++c1.y();++c2.y();}
			if(vec.y()<0){--c3.y();--c4.y();}
		}
		//开始积分
		auto aa=radius.x()*radius.x();
		auto bb=radius.y()*radius.y();
		auto ab=radius.x()*radius.y();
		decltype(rect.p0) p(0,radius.y());
		auto upLimit = ab*ab + ab;
		//X方向积分
		while(p.x()*bb<=p.y()*aa){
			pixmap.setColor(c1.x()+p.x(),c1.y()+p.y(),color);
			pixmap.setColor(c2.x()-p.x(),c2.y()+p.y(),color);
			pixmap.setColor(c3.x()-p.x(),c3.y()-p.y(),color);
			pixmap.setColor(c4.x()+p.x(),c4.y()-p.y(),color);
			++p.x();
			if(p.x()*p.x()*bb + p.y()*p.y()*aa > upLimit){
				--p.y();
			}
		}
		//Y方向积分
		p.x()=radius.x();p.y()=0;
		while(p.y()*aa<=p.x()*bb){
			pixmap.setColor(c1.x()+p.x(),c1.y()+p.y(),color);
			pixmap.setColor(c2.x()-p.x(),c2.y()+p.y(),color);
			pixmap.setColor(c3.x()-p.x(),c3.y()-p.y(),color);
			pixmap.setColor(c4.x()+p.x(),c4.y()-p.y(),color);
			++p.y();
			if(p.x()*p.x()*bb + p.y()*p.y()*aa > upLimit){
				--p.x();
			}
		}
	}
	//画椭圆(圆心,x半径,y半径)
	void drawEllipse(const Point2D<int> &center,int rx,int ry){
		drawEllipse(Rectangle2D<int>(
			Point2D<int>(center.x()-rx,center.y()-ry),
			Point2D<int>(center.x()+rx,center.y()+ry)
		));
	}
	//画圆(圆心,半径)椭圆版
	void drawEllipse(const Point2D<int> &center,int r){
		drawEllipse(center,r,r);
	}
	//Bezier曲线结构{起点,控制点,终点}
	struct BezierCurve{
		Point2D<int> start,control,finish;
	};
	//画Bezier曲线(起点,控制点,终点)
	void drawBezier(const Point2D<int> start,const Point2D<int> &control,const Point2D<int> &finish){
		//printf("drawBezier: %s--%s--%s\n",start.toString().data(),control.toString().data(),finish.toString().data());
		
		list< BezierCurve > allCurves;
		//BezierCurve curve={start,control,finish};
		allCurves.push_back({start,control,finish});
		auto itr=allCurves.begin();
		Point2D<int> p0,p1,p2;
		while(itr!=allCurves.end()){
			//找特征点(中点)
			p0=(itr->start + itr->control)/2;
			p2=(itr->control + itr->finish)/2;
			p1=(p0+p2)/2;
			//printf("\t\tp0=%s p1=%s p2=%s\n",p0.toString().data(),p1.toString().data(),p2.toString().data());
			//画前半段曲线
			if((p1 - itr->start).distance2()<=16){
				drawLine(itr->start,p1);
			}else{
				allCurves.push_back({itr->start,p0,p1});
			}
			//画后半段曲线
			if((itr->finish-p1).distance2()<=16){
				drawLine(p1,itr->finish);
			}else{
				allCurves.push_back({p1,p2,itr->finish});
			}
			//下一个
			itr=allCurves.erase(itr);
		};
	}
	//画Bezier曲线(点列表),点列表中的首项和末项分别是起点和终点,其余都是控制点
	void drawBezier(const list< Point2D<int> > &points){
		auto sz=points.size();
		if(sz<3){
			if(sz==1){
				pixmap.setColor(*points.begin(),color);
			}else if(sz==2){
				drawLine(*points.begin(),*points.rbegin());
			}
			return;
		}
		//至少要3个点
		auto controlPoints=points;//全部控制点(points去掉首尾项)
		controlPoints.pop_front();
		controlPoints.pop_back();
		//由控制点推出线上点
		decltype(controlPoints) linePoints;
		auto itrA=controlPoints.begin();
		auto itrB=itrA;++itrB;
		while(itrB != controlPoints.end()){
			linePoints.push_back((*itrA + *itrB)/2);
			//下一个
			itrA=itrB;++itrB;
		}
		linePoints.push_front(*points.begin());
		linePoints.push_back(*points.rbegin());
		//嗯有了线上点和控制点就可以画曲线了
		itrA=linePoints.begin();
		itrB=itrA;++itrB;
		auto itrC=controlPoints.begin();
		while(itrB != linePoints.end() && itrC != controlPoints.end()){
			drawBezier(*itrA,*itrC,*itrB);
			//下一个
			itrA=itrB;++itrB;++itrC;
		}
	}

	Pixmap<ColorType> &pixmap;
	Point2D<int> startPoint;
	ColorType color;
};

#endif
