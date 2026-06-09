#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


const char stenoOrder[] = "STKPWHR*AOEUFRPBLGTSDZ#";

const int buttonPins[] = {
  2,3,4,6,7,8,9,10,11,12,13,
  22,23,24,25,26,27,28,29,30,31,32,33
};

const int numButtons = sizeof(buttonPins)/sizeof(buttonPins[0]);
const int clearButton = 5;


struct Entry { 
  const char* stroke; 
  const char* word; 
};

Entry dictionary[] = {

{"EU","I"},{"U","you"},{"HE","he"},{"SHE","she"},{"WE","we"},
{"THEU","they"},{"ME","me"},{"HER","her"},{"HIM","him"},
{"US","us"},{"THEM","them"},{"PWAEU","my"},{"YOUR","your"},
{"OUR","our"},{"THEUR","their"},

{"PW","be"},{"R","are"},{"WAS","was"},{"WERE","were"},
{"TPH","in"},{"HAVE","have"},{"HAD","had"},{"DO","do"},
{"TK","did"},{"SAEU","say"},{"GO","go"},{"GET","get"},
{"PHAKE","make"},{"TPHAOE","know"},{"THAEU","think"},
{"TAKE","take"},{"SE","see"},{"KPL","come"},{"WAUPBT","want"},
{"LOOK","look"},{"USE","use"},{"TE","tell"},{"GIVE","give"},
{"WORK","work"},{"TRY","try"},{"LEAVE","leave"},{"CALL","call"},
{"NEED","need"},{"FEEL","feel"},{"SEEM","seem"},{"ASK","ask"},
{"FIPBD","find"},{"KEEP","keep"},{"LET","let"},{"PUT","put"},
{"BEGIN","begin"},{"HELP","help"},{"TALK","talk"},{"TURN","turn"},
{"START","start"},{"SHOW","show"},{"HEAR","hear"},{"PLAY","play"},
{"RUN","run"},{"MOVE","move"},{"LIVE","live"},{"BELIEVE","believe"},
{"HOLD","hold"},{"BRING","bring"},{"HAPPEN","happen"},
{"WRITE","write"},{"PROVIDE","provide"},{"SIT","sit"},
{"STAND","stand"},{"LOSE","lose"},{"PAY","pay"},{"MEET","meet"},
{"INCLUDE","include"},{"CONTINUE","continue"},{"SET","set"},
{"LEARN","learn"},{"CHANGE","change"},{"LEAD","lead"},
{"UNDERSTAND","understand"},{"WATCH","watch"},{"FOLLOW","follow"},
{"STOP","stop"},{"CREATE","create"},{"SPEAK","speak"},
{"READ","read"},{"ALLOW","allow"},{"ADD","add"},
{"SPEND","spend"},{"GROW","grow"},{"OPEN","open"},
{"WALK","walk"},{"WIN","win"},{"OFFER","offer"},
{"REMEMBER","remember"},{"LOVE","love"},{"CONSIDER","consider"},
{"APPEAR","appear"},{"BUY","buy"},{"WAIT","wait"},
{"SERVE","serve"},{"DIE","die"},{"SEND","send"},
{"EXPECT","expect"},{"BUILD","build"},{"STAY","stay"},
{"FALL","fall"},{"CUT","cut"},{"REACH","reach"},
{"KILL","kill"},{"REMAIN","remain"},

{"TIME","time"},{"PERSON","person"},{"YEAR","year"},
{"WAY","way"},{"DAY","day"},{"THING","thing"},
{"MAN","man"},{"WORLD","world"},{"LIFE","life"},
{"HAND","hand"},{"PART","part"},{"CHILD","child"},
{"EYE","eye"},{"WOMAN","woman"},{"PLACE","place"},
{"WORK","work"},{"WEEK","week"},{"CASE","case"},
{"POINT","point"},{"GOVERNMENT","government"},
{"COMPANY","company"},{"NUMBER","number"},
{"GROUP","group"},{"PROBLEM","problem"},
{"FACT","fact"},

{"THE","the"},{"AND","and"},{"OR","or"},{"BUT","but"},
{"IF","if"},{"BECAUSE","because"},{"AS","as"},
{"UNTIL","until"},{"WHILE","while"},{"OF","of"},
{"AT","at"},{"BY","by"},{"FOR","for"},{"WITH","with"},
{"ABOUT","about"},{"AGAINST","against"},{"BETWEEN","between"},
{"INTO","into"},{"THROUGH","through"},{"DURING","during"},
{"BEFORE","before"},{"AFTER","after"},{"ABOVE","above"},
{"BELOW","below"},{"FROM","from"},{"UP","up"},
{"DOWN","down"},{"OUT","out"},{"ON","on"},
{"OFF","off"},{"OVER","over"},{"UNDER","under"},
{"THEN","then"},{"ONCE","once"},{"HERE","here"},
{"THERE","there"},{"WHEN","when"},{"WHERE","where"},
{"WHY","why"},{"HOW","how"},{"ALL","all"},
{"ANY","any"},{"BOTH","both"},{"EACH","each"},
{"FEW","few"},{"MORE","more"},{"MOST","most"},
{"OTHER","other"},{"SOME","some"},{"SUCH","such"},
{"NO","no"},{"NOR","nor"},{"NOT","not"},
{"ONLY","only"},{"OWN","own"},{"SAME","same"},
{"SO","so"},{"THAN","than"},{"TOO","too"},
{"VERY","very"},

{"KAT","cat"},{"TKOG","dog"},{"HOR","horse"},{"KAU","cow"},
{"SHEP","sheep"},{"GOES","goat"},{"PIG","pig"},{"DUK","duck"},
{"CHIK","chicken"},{"TAOEG","tiger"},{"LAEUPB","lion"},{"BEAR","bear"},
{"FISH","fish"},{"BIRD","bird"},{"MOUSE","mouse"},{"RABT","rabbit"},
{"ELEPH","elephant"},{"MONK","monkey"},{"SNAEK","snake"},{"FROG","frog"},{"SEAL","seal"},

{"#S","1"},{"#T","2"},{"#P","3"},{"#H","4"},
{"#A","5"},{"#F","6"},{"#PL","7"},{"#L","8"},
{"#D","9"},{"#O","0"},

{"HROE","hello"},{"THA*U","thank you"},
{"GO*MRG","good morning"},{"GO*AFRN","good afternoon"},
{"GO*EVN","good evening"},{"GN*","good night"},
{"HAU","how are you"},{"IMF","I'm fine"},
{"SOR*","sorry"},{"EXCU*","excuse me"},
{"PL*","please"},{"WHTS*NEW","what's new"},
{"C*U","see you"},{"BY*","bye"},
{"GOODBY*","goodbye"},{"WEL*","welcome"},
{"CONG*","congratulations"},{"HBD*","happy birthday"},
{"HN*Y","happy new year"},{"HG*DAY","have a good day"},
{"TAK*CARE","take care"},{"N2M*U","nice to meet you"},
{"SEE*YA","see ya"},{"L*TMS*","long time no see"},
{"GDL*","good luck"},{"GWS*","get well soon"},
{"HANN*","happy anniversary"},{"HHOL*","happy holidays"},
{"BWSH*","best wishes"},{"PWAPB","ban"}

};

const int dictSize = sizeof(dictionary)/sizeof(dictionary[0]);


bool currentStroke[numButtons] = {0};
bool keyWasPressed = false;
String fullSentence = "";


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("STENO READY");

  for(int i=0;i<numButtons;i++)
    pinMode(buttonPins[i], INPUT_PULLUP);

  pinMode(clearButton, INPUT_PULLUP);
}


void loop() {

  if(digitalRead(clearButton)==LOW){
    fullSentence="";
    lcd.clear();
    lcd.print("RESET");
    delay(500);
    lcd.clear();
    lcd.print("READY");
    return;
  }

  bool anyKeyDown=false;

  for(int i=0;i<numButtons;i++){
    if(digitalRead(buttonPins[i])==LOW){
      currentStroke[i]=true;
      anyKeyDown=true;
    }
  }

  if(keyWasPressed && !anyKeyDown){
    delay(120);
    processStroke();
    keyWasPressed=false;
  }

  keyWasPressed=anyKeyDown;
}


void processStroke(){

  String raw="";
  int numberIndex=strlen(stenoOrder)-1;
  bool isNumber=currentStroke[numberIndex];

  if(isNumber) raw+="#";

  for(int i=0;i<numButtons;i++){
    if(i==numberIndex) continue;
    if(currentStroke[i]) raw+=stenoOrder[i];
  }

  if(raw=="*"){
    backspace();
  }
  else if(raw.length()>0){
    String translated=translate(raw);
    fullSentence+=translated+" ";
    updateDisplay(raw,translated);
  }

  for(int i=0;i<numButtons;i++)
    currentStroke[i]=false;
}

String translate(String raw){
  for(int i=0;i<dictSize;i++){
    if(raw==dictionary[i].stroke)
      return dictionary[i].word;
  }
  return "["+raw+"]";
}


void backspace(){
  int lastSpace=fullSentence.lastIndexOf(' ',fullSentence.length()-2);
  fullSentence=(lastSpace!=-1)?
  fullSentence.substring(0,lastSpace+1):"";
  updateDisplay("*DEL*","---");
}


void updateDisplay(String raw,String translated){

  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print(raw + ">" + translated);

  lcd.setCursor(0,1);
  lcd.print("                ");
  int start=max(0,(int)fullSentence.length()-16);
  lcd.setCursor(0,1);
  lcd.print(fullSentence.substring(start));
}
