#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define pin_leds        19 // On Trinket or Gemma, suggest changing this to 1
#define numpixels 5 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(numpixels, pin_leds, NEO_GRB + NEO_KHZ800);

#define MPU 0x68

int16_t ax=0;
int16_t ay=0;
int16_t az=0;
int16_t gx=0;
int16_t gy=0;
int16_t gz=0;
int16_t temp=0;

int16_t dt=0;
int16_t tiempo_prev=0;


float girosc_ang_x, girosc_ang_y;
float girosc_ang_x_prev, girosc_ang_y_prev;


#define pin_enable_xye 2


#define pin_x_dire 15
#define pin_x_step 13
#define pin_y_dire 4
#define pin_y_step 16

const char* ssid = "robotlimpiafondos";
const char* password = "robotlimpiafondos";


WiFiServer server(80);
WiFiClient client;



int posicion=0;

int angulo=0;

int i=0;

int gzruido=0;

String s="";

int estadomotores=0;

int contadormotores=0;

int px=0;
int py=0;

int sensor_delantero=0;
int sensor_trasero=0;
int sensor_izquierdo=0;
int sensor_derecho=0;

void cerrarcliente(){
  client.flush();
  client.stop();   
//  client.stopAll();     
}

void cerrarconexiones(){
  bool cliente=true;

  while(cliente) {    
      delay(100);  
      client=server.available();
      cliente=false;
      
      if(client){
        if(client.connected()){
          if(client.available()){ 
              cliente=true;             
              cerrarcliente();             
          } 
        }
      }
  } 
  
}

void enviavalores() {

    s="";
    s+=ax;s+=";";
    s+=ay;s+=";";
    s+=az;s+=";";
    s+=temp;s+=";";
    s+=angulo;s+=";";
    s+=px;s+=";";
    s+=py;s+=";";
    s+=sensor_delantero;s+=";";
    s+=sensor_derecho;s+=";";
    s+=sensor_trasero;s+=";";
    s+=sensor_izquierdo;s+=";";

    client.print(s);
    
}


void clienteweb() {

  Serial.println("Se recibe peticion");

  String req;
  int contador=0;
  while(!client.connected()) {delay(1);contador++;if(contador>1000){return;}}
  contador=0;
  while(!client.available()) {delay(1);contador++;if(contador>1000){return;}}

  char c;
  do{          
    c = client.read();
    req+=c;          
  }while(c!='\n');


  if(req.indexOf("/adelante")!=-1){
      contadormotores=0;
      estadomotores=1;  
      enviavalores();
  
  }else if(req.indexOf("/atras")!=-1){
    
      contadormotores=0;
      estadomotores=2;  
      enviavalores();
  
  }else if(req.indexOf("/izquierda")!=-1){
    
      contadormotores=0;
      estadomotores=3;  
      enviavalores();
  
  }else if(req.indexOf("/derecha")!=-1){
    
      contadormotores=0;
      estadomotores=4;  
      enviavalores();
  
  }else if(req.indexOf("/actualiza")!=-1){
    
      enviavalores();
  
  }else if (req.indexOf("/ ")!=-1){
    
    s=F("<html>\n\n<head><meta http-equiv='Content-Type' content='text/html;charset=UTF-8'><title id='titulo'>ROBOT LIMPIAFONDOS TURMANDREAMS</title>\n<script type='text/javascript'>\n\nvar isMobile={\nmobilecheck : function(){\nreturn(/(android|bb\\d+|meego).+mobile|avantgo|bada\\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\\.(browser|link)|vodafone|wap|wis (ce|phone)|xda|xkno|android|ipad|playbook|silk/i.test(navigator.userAgent||navigator.vendor||wi.opera)||/1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\\-(n|u)|c55\\/|capi|ccwa|cdm\\-|cell|chtm|cldc|cmd\\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\\-s|devi|dica|dmob|do(c|p)o|ds(12|\\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\\-|_)|g1 u|g560|gene|gf\\-5|g\\-mo|go(\\.w|od)|gr(ad|un)|haie|hcit|hd\\-(m|p|t)|hei\\-|hi(pt|ta)|hp( i|ip)|hs\\-c|ht(c(\\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\\-(20|go|ma)|i230|iac( |\\-|\\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\\/)|klon|kpt |kwc\\-|kyo(c|k)|le(no|xi)|lg( g|\\/(k|l|u)|50|54|\\-[a-w])|libw|lynx|m1\\-w|m3ga|m50\\/|ma(te|ui|xo)|mc(01|21|ca)|m\\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\\-2|po(ck|rt|se)|prox|psio|pt\\-g|qa\\-a|qc(07|12|21|32|60|\\-[2-7]|i\\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\\-|oo|p\\-)|sdk\\/|se(c(\\-|0|1)|47|mc|nd|ri)|sgh\\-|shar|sie(\\-|m)|sk\\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\\-|v\\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\\-|tdg\\-|tel(i|m)|tim\\-|t\\-mo|to(");client.print(s);
    s=F("pl|sh)|ts(70|m\\-|m3|m5)|tx\\-9|up(\\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\\-|your|zeto|zte\\-/i.test((navigator.userAgent||navigator.vendor||wi.opera).substr(0,4)))\n}\n}\n\n\nvar movil=false;\nvar firefox=false;\nvar wtx=0;\nvar wty=0;\n\nvar xc=0;\nvar xy=0;\n\nvar xc2=0;\nvar xy2=0;\n\nvar px=0;\nvar py=0;\n\nvar pwtx=0;\n\nvar wi=window;\n\nvar rx;\nvar gx;\nvar bx;\n\nvar rr=128;\nvar gg=128;\nvar bb=128;\n\nvar R=0;\n\nvar sz;\n\nvar moviendo=false;\nvar rpulsado=false;\n\nvar posicionrobotx=0;\nvar posicionroboty=0;\n\nvar X = 0;\nvar Y = 0;\nvar R = 0;\nvar ap = 0;\nvar af = 0;\n\nvar Xap = X+R * Math.cos(ap);var Yap = Y+R * Math.sin(ap);\n\nvar parpadeo=true;\n\nvar contador=0;\n\nfunction dg(z){return document.getElementById(z);}\n\nfunction init(fps){\n    \nvar navegador=navigator.userAgent;\nvar nav=navegador.toLowerCase(); \n\nif(nav.indexOf('firefox')!=-1){firefox=true;}\n\nmovil=isMobile.mobilecheck();\n\nc=dg('c');\n\nif(!firefox){\nc.addEventListener('touchstart',rp,false);\nc.addEventListener('touchmove',rm,false);\nc.addEventListener('touchend',rpu,false);\n}\n\nc.addEventListener('mousedown',rp,false);\nc.addEventListener('mousemove',rm,false);\nc.addEventListener('mouseup',rpu,false);\n\n\na();\n\n\nif(c.getContext){\ng=c.getContext('2d');g.canvas.width=xc;g.canvas.height=yc;\nsetInterval(upd,300);\n    }\n\nrx=xc/2;\ngx=xc/2;\nbx=xc/2;\n\n\nR=xc/100;\n}\n\nfunction get(peticion){\n\npx=0;py=0;\nip=location.host;\n\nif(wi.XMLHttpRequest){xmlhttp=new XMLHttpRequest();}\nelse{xmlhttp=new ActiveXObject('Microsoft.XMLHTTP');}\n\nxmlhttp.onreadystatechange=function(){\nif(xmlhttp.responseText!='' && xmlhttp.readyState==4 && xmlhttp.status==200){\ndatorecogido=xmlhttp.responseText;\n\n///debemos procesar los datos para poder actualizar los valores.\n\n//alert(datorecogido);\n\n}else if(xmlhttp.status==404){alert('FALLO COMUNICACION !!');}\n}\n\n//alert(peticion);\n\nxmlhttp.open('G");client.print(s);
    s=F("ET','http://'+ip+'/'+peticion,true);\nxmlhttp.send();\n\n\n}\n\nfullscreen = function(e){\n      if (e.webkitRequestFullScreen) {e.webkitRequestFullScreen();}\n  else if(e.mozRequestFullScreen) {e.mozRequestFullScreen();}\n}\n\nfunction mm(e){\nif((movil)&&(!firefox)){\npx=e.touches[0].pageX-pwtx;\npy=e.touches[0].pageY-7;\n}else{\npx=e.clientX-pwtx;\npy=e.clientY+wi.scrollY-7;\n}\n}\n\nfunction rpu(e){ px=0;py=0;rpulsado=false;upd();}\n\nfunction rp(e){ mm(e);rpulsado=true;contador=0;upd();}\n\nfunction rm(e){\nif(rpulsado) {\nmoviendo=true;\nmm(e);upd();\nmoviendo=false;\n}\n}\n\nfunction a(){\n\nif((wtx!=wi.innerWidth)||(wty!=wi.innerHeight)){\n\nwtx=wi.innerWidth;\nwty=wi.innerHeight;\n\nxc=wtx*0.98;\nyc=wty*0.97;\n\npwtx=((wtx-xc)/2);\n\nc.style.left=pwtx;\nc.width=xc;c.height=yc;\n\n}\n\n\n}\n\nfunction gb(){g.beginPath();}\nfunction gf(){g.fill();}\nfunction gc(){g.closePath();}\nfunction gs(){g.stroke();}\nfunction gm(x,y){g.moveTo(x,y);}\nfunction gl(x,y){g.lineTo(x,y);}\nfunction gsensor(){gm(X,Y);g.lineTo(Xap,Yap);g.arc(X,Y,R,ap,af);g.closePath();}\n\nfunction r(v,x,y,tx,ty){gb();g.rect(x,y,tx,ty);g.fillStyle=v;gf();g.lineWidth=2;g.strokeStyle='black';gs();gc();}\n\n\nfunction upd(){\n\n\ncontador++;\n\nif(contador>3) { contador=0; get('actualiza'); }\n\na();\nr('#DDDDDD',0,0,xc,yc);\nr('#000000',px,py,2,2);\ngf();\ngs();\ngc();\n\n\n//pintamos donde creemos que está el robot\n\nposicionrobotx=Math.random() * ((xc*60/100) - (xc*40/100)) + (xc*40/100);\nposicionroboty=Math.random() * ((yc*60/100) - (yc*40/100)) + (yc*40/100);\n\n\n///Sensor de Inclinación\n\nvar inclinacionx='Inclinacion X : 20';\nvar inclinaciony='Inclinacion Y : 20';\nvar inclinacionz='Inclinacion Z : 2000';\nvar brujula='Brujula : 90 Grados';\nvar temperatura='Temp : 20 Grados';\n\ngb();\nr('#FFFFFF',(xc*10)/1000,(yc*20)/1000,(xc*220)/1000,(yc*300)/1000);\ngf();\ngs();\ngc();\n\ngb();\nsz=xc/50;\ng.fillStyle = '#000000';\ng.font='bold '+sz+'px Arial';\ng.fillText(inclinacionx,((xc*25)/1000),(yc*8)/");client.print(s);
    s=F("100);\ng.fillText(inclinaciony,((xc*25)/1000),(yc*13)/100);\ng.fillText(inclinacionz,((xc*25)/1000),(yc*18)/100);\ng.fillText(brujula,((xc*25)/1000),(yc*23)/100);\ng.fillText(temperatura,((xc*25)/1000),(yc*28)/100);\ngf();\ngs();\n\n\n///Mensajes Importantes\n\nvar mensaje='Funcionando correctamente !!';\n\ngb();\nr('#FFFFFF',(xc*630)/1000,(yc*20)/1000,(xc*300)/1000,(yc*200)/1000);\ngf();\ngs();\ngc();\n\nif(parpadeo) {\n\ngb();\nsz=xc/60;\ng.fillStyle = '#000000';\ng.font='bold '+sz+'px Arial';\ng.fillText(mensaje,((xc*650)/1000),(yc*13)/100);\ngf();\ngs();\n\n}\nparpadeo=!parpadeo;\n\n\n//Dibujamos piscina y posicion estimada robot\n\ngb();\nr('#DDDDFF',xc/4,yc/4,xc/2,yc/2);\ngf();\ngs();\ngc();\n\n\ngb();\ng.strokeStyle = '#006400';\ng.fillStyle = '#ffdd0a';\ng.lineWidth = 5;\ng.arc(posicionrobotx,posicionroboty,xc/50,0,2*Math.PI);\ngf();\ngs();\n\ngb();\nsz=xc/50;\ng.fillStyle = '#000000';\ng.font='bold '+sz+'px Arial';\ng.fillText('R',posicionrobotx-(xc/150),posicionroboty+(yc/100));\ngf();\ngs();\n\n\n\n//Sensores\n\nR+=xc/100;\n\nif(R>xc/20) { R=xc/100;}\n\n//Sensor arriba\n\ngb();\nX = xc/2;Y = (yc*15)/100;\n\nap = (Math.PI / 180) * 240;af = (Math.PI / 180) * 300;\n\nXap = X+R * Math.cos(ap);Yap = Y+R * Math.sin(ap);\n\ng.fillStyle = '#abcdef';g.strokeStyle = '#1E90FF';\ng.lineWidth = 5;\ngsensor();\ngf();\ngs();\n\ngb();\nsz=xc/50;\ng.fillStyle = '#000000';\ng.font='bold '+sz+'px Arial';\ng.fillText('2500',xc/2-(xc/45),(yc*15)/100+(yc/30));\ngf();\ngs();\n\n\n//Sensor abajo\n\ngb();\nX = xc/2;Y = (yc*85)/100;\n\nap = (Math.PI / 180) * 60;af = (Math.PI / 180) * 120;\n\nXap = X+R * Math.cos(ap);Yap = Y+R * Math.sin(ap);\n\ng.fillStyle = '#abcdef';g.strokeStyle = '#1E90FF';\ng.lineWidth = 5;\ngsensor();\ngf();\ngs();\n\ngb();\nsz=xc/50;\ng.fillStyle = '#000000';\ng.font='bold '+sz+'px Arial';\ng.fillText('2500',xc/2-(xc/45),(yc*85)/100-(yc/200));\ngf();\ngs();\n\n//Sensor izquierdo\n\ngb();\nX = (xc*10)/100;Y = yc/2;\n\nap = (Math.PI / 180) * 150;af = (Math.PI / 180) * ");client.print(s);
    s=F("210;\n\nXap = X+R * Math.cos(ap);Yap = Y+R * Math.sin(ap);\n\ng.fillStyle = '#abcdef';g.strokeStyle = '#1E90FF';\ng.lineWidth = 5;\ngsensor();\ngf();\ngs();\n\ngb();\nsz=xc/50;\ng.fillStyle = '#000000';\ng.font='bold '+sz+'px Arial';\ng.fillText('2500',((xc*105)/1000),(yc*51)/100);\ngf();\ngs();\n\n\n//Sensor derecho\n\ngb();\nX = (xc*90)/100;Y = yc/2;\n\nap = (Math.PI / 180) * 330;af = (Math.PI / 180) * 30;\n\nXap = X+R * Math.cos(ap);Yap = Y+R * Math.sin(ap);\n\ng.fillStyle = '#abcdef';g.strokeStyle = '#1E90FF';\ng.lineWidth = 5;\ngsensor();\ngf();\ngs();\n\ngb();\nsz=xc/50;\ng.fillStyle = '#000000';\ng.font='bold '+sz+'px Arial';\ng.fillText('2500',((xc*850)/1000),(yc*51)/100);\ngf();\ngs();\n\n\n//Controles de dirección\n\ngb();\nr('#FFFFFF',(xc*85)/1000,(yc*600)/1000,xc/15,yc/7);//Flecha arriba\ngf();\ngs();\n\nif(rpulsado) {\n\nif((px>((xc*85)/1000))&&(px<(((xc*85)/1000)+(xc/15)))) {\nif((py>((yc*600)/1000))&&(py<(((yc*600)/1000)+(yc/7)))) {\n\nrpulsado=false;\n//se ha pulsado arriba\n\nget('adelante');\n\n//alert('arriba');\n}\n}\n}\n\n\ngb();\ng.lineWidth = 5;\ng.strokeStyle = '#000000';\ng.fillStyle = '#0000FF';\ng.moveTo((xc*120)/1000,(yc*610)/1000);\ng.lineTo((xc*90)/1000,(yc*725)/1000);\ng.lineTo((xc*145)/1000,(yc*725)/1000);\ng.lineTo((xc*120)/1000,(yc*610)/1000);\ngf();\ngs();\n\n\ngb();\nr('#FFFFFF',(xc*85)/1000,(yc*800)/1000,xc/15,yc/7);//Flecha abajo\ngf();\ngs();\n\nif(rpulsado) {\n\nif((px>((xc*85)/1000))&&(px<(((xc*85)/1000)+(xc/15)))) {\nif((py>((yc*800)/1000))&&(py<(((yc*800)/1000)+(yc/7)))) {\n\n//se ha pulsado abajo\nrpulsado=false;\nget('atras');\n\n//alert('abajo');\n}\n}\n}\n\ngb();\ng.lineWidth = 5;\ng.strokeStyle = '#000000';\ng.fillStyle = '#0000FF';\ng.moveTo((xc*120)/1000,(yc*935)/1000);\ng.lineTo((xc*90)/1000,(yc*815)/1000);\ng.lineTo((xc*145)/1000,(yc*815)/1000);\ng.lineTo((xc*120)/1000,(yc*935)/1000);\ngf();\ngs();\n\n\ngb();\nr('#FFFFFF',(xc*800)/1000,(yc*700)/1000,xc/15,yc/7);//Flecha izquierda");client.print(s);
    s=F("\ngf();\ngs();\n\nif(rpulsado) {\n\nif((px>((xc*800)/1000))&&(px<(((xc*800)/1000)+(xc/15)))) {\nif((py>((yc*700)/1000))&&(py<(((yc*700)/1000)+(yc/7)))) {\n\n//se ha pulsado izquierda\nrpulsado=false;\nget('izquierda');\n\n//alert('izquierda');\n}\n}\n}\n\ngb();\ng.lineWidth = 5;\ng.strokeStyle = '#000000';\ng.fillStyle = '#0000FF';\ng.moveTo((xc*805)/1000,(yc*770)/1000);\ng.lineTo((xc*860)/1000,(yc*710)/1000);\ng.lineTo((xc*860)/1000,(yc*830)/1000);\ng.lineTo((xc*805)/1000,(yc*770)/1000);\ngf();\ngs();\n\n\ngb();\nr('#FFFFFF',(xc*900)/1000,(yc*700)/1000,xc/15,yc/7);//Flecha derecha\ngf();\ngs();\n\nif(rpulsado) {\n\nif((px>((xc*900)/1000))&&(px<(((xc*900)/1000)+(xc/15)))) {\nif((py>((yc*700)/1000))&&(py<(((yc*700)/1000)+(yc/7)))) {\n\n//se ha pulsado derecha\nrpulsado=false;\nget('derecha');\n\n//alert('derecha');\n}\n}\n}\n\ngb();\ng.lineWidth = 5;\ng.strokeStyle = '#000000';\ng.fillStyle = '#0000FF';\ng.moveTo((xc*963)/1000,(yc*770)/1000);\ng.lineTo((xc*905)/1000,(yc*710)/1000);\ng.lineTo((xc*905)/1000,(yc*830)/1000);\ng.lineTo((xc*963)/1000,(yc*770)/1000);\ngf();\ngs();\n\n\n}\n\n</script></head><body onload='init(30)' id='bodi' style='overflow-y:hidden;overflow-x:hidden'>\n<center><canvas id='c' tabindex='0' style='border:1px solid #000000;'></canvas></center></body></html>");client.print(s);

 
    
  }else{
    s=F("<html><body><center><img src='http://www.turmandreams.es/3dprinter/logo.gif' /></center></body></html>");client.print(s);
  }

  client.print("\r\n\r\n");      


  s=F("");
  cerrarconexiones();

  //Serial.println(gcode);
  
  
  

}

void updateGiro()
{
   dt = millis() - tiempo_prev;
   tiempo_prev = millis();
   girosc_ang_x = (gx / 131)*dt / 1000.0 + girosc_ang_x_prev;
   girosc_ang_y = (gy / 131)*dt / 1000.0 + girosc_ang_y_prev;
   girosc_ang_x_prev = girosc_ang_x;
   girosc_ang_y_prev = girosc_ang_y;
}


void leempu6050() {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true); //A partir del 0x3B, se piden 6 registros
  while(Wire.available() < 14);
  
  ax=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
  ay=Wire.read()<<8|Wire.read();
  az=Wire.read()<<8|Wire.read();
  
  temp=Wire.read()<<8|Wire.read();
  
  gx=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
  gy=Wire.read()<<8|Wire.read();
  gz=Wire.read()<<8|Wire.read();

  //int gz2=gz-gzruido;

  //Serial.println(gz2);

  String cadena="";
  cadena+=ax;cadena+=";";
  cadena+=ay;cadena+=";";

  //Serial.println(cadena);

  int posicion2=0;
  if(ay<-200) { posicion2=1; }
  else if(ay>200) { posicion2=2; }
  else if(ax<-200) { posicion2=3; }
  else if(ax>200) { posicion2=4; }
  else { posicion2=0; }
/*
  if(posicion2!=posicion) {
    pixels.clear();
  
      for(i=0;i<numpixels;i++) {
        pixels.setPixelColor(i,pixels.Color(0,0,200)); 
    }

    
    if(posicion2==1) { pixels.setPixelColor(0,pixels.Color(200,0,0)); }
    else if(posicion2==2) { pixels.setPixelColor(2,pixels.Color(200,0,0)); }
    else if(posicion2==3) { pixels.setPixelColor(1,pixels.Color(200,0,0)); }
    else if(posicion2==4) { pixels.setPixelColor(3,pixels.Color(200,0,0)); }
    else { pixels.setPixelColor(4,pixels.Color(200,0,0)); }

    pixels.show();
  }
  posicion=posicion2;
  */
}

void setup() {
  
   Serial.begin(115200);

   pinMode (pin_leds,OUTPUT); 
   
   pinMode (pin_enable_xye,OUTPUT); 
   digitalWrite(pin_enable_xye,HIGH);
  
   pinMode(pin_x_step,OUTPUT);  
   pinMode(pin_x_dire,OUTPUT); 
  
   pinMode(pin_y_step,OUTPUT); 
   pinMode(pin_y_dire,OUTPUT); 

   WiFi.softAP(ssid, password);        //Start Acces point mode
  // WiFi.softAPConfig(local_ip, gateway, subnet);
   delay(500);

   server.setNoDelay(true);
   server.begin();   
        

   Wire.begin(21,22);
  
   Wire.beginTransmission(MPU);
   Wire.write(0x6B);
   Wire.write(0);
   Wire.endTransmission(true);
  
   Wire.beginTransmission(MPU);
   Wire.write(0x19);   //Sample Rate
   Wire.write(0x00);   //  8000 / 1 + 0
   Wire.endTransmission(true);
  
   Wire.beginTransmission(MPU);
   Wire.write(0x1C);   //Setting Accel
   Wire.write(0x18);      //  + / - 16g
   Wire.endTransmission(true);
  
   delay(200);

   gz=0;
   for(i=0;i<100;i++){
      leempu6050();
      gzruido+=gz;
      delay(10);
   }
   gzruido=gzruido/100;

 //  delay(500);

  pixels.clear();
  

  for(i=0;i<numpixels;i++) {
      pixels.setPixelColor(i,pixels.Color(0,0,200)); 
  }
  
  pixels.show();
   
}



void loop() {



  client = server.available();
  if (client) {  //Si hay petición del cliente   
        clienteweb();
  }  
  
    
  //probamos motores
  contadormotores++;
  if(contadormotores>1000) { contadormotores=0;estadomotores=0; }
    
  if(estadomotores==0) { digitalWrite(pin_enable_xye,HIGH); }
  else { digitalWrite(pin_enable_xye,LOW); }

  if(estadomotores==1) { 
    digitalWrite(pin_x_dire,LOW);  
    digitalWrite(pin_y_dire,LOW);    
  }else if(estadomotores==2) { 
    digitalWrite(pin_x_dire,HIGH);  
    digitalWrite(pin_y_dire,HIGH);    
  }else if(estadomotores==3) { 
    digitalWrite(pin_x_dire,HIGH);  
    digitalWrite(pin_y_dire,LOW);    
  }else if(estadomotores==4) { 
    digitalWrite(pin_x_dire,LOW);  
    digitalWrite(pin_y_dire,HIGH);    
  }
  
   
  
  
  
 
  /*
  pixels.clear();
  delay(10);

  for(i=0;i<numpixels;i++) {
      pixels.setPixelColor(i,pixels.Color(0,0,200)); 
  }
    */
  
  leempu6050();

  //pixels.show();

  //delay(10);

  
  

  sensor_izquierdo=analogRead(27); // Sensor Izquierda  

  sensor_trasero=analogRead(26); // Sensor Atras
  
  sensor_derecho=analogRead(25); // Sensor Derecha  

  sensor_delantero=analogRead(34); // Sensor Adelante
      

  //Mueve los motores y produce un delay de unos 10 ms

   for(i=0;i<24;i++) { //12800
    digitalWrite(pin_x_step, HIGH); 
    digitalWrite(pin_y_step, HIGH);
    delayMicroseconds(200);   
    digitalWrite(pin_x_step, LOW); 
    digitalWrite(pin_y_step, LOW); 
    delayMicroseconds(200);   
    yield();
  }
  
}
