
char KeyBuf[256];
struct Enemy;
std::vector<Enemy> enemies, add_enemies;
unsigned int EnCount = 0U;
int PlayerHp = 640;

inline bool isin( int x1, int x2, int v ){
	return static_cast<unsigned int>(x2 - x1) > static_cast<unsigned int>( v - x1 );
}
inline int pow2( int v ){
	return v*v;
}
inline bool isin( int x1, int y1, int x2, int y2, int r ){
	return (pow2(y2-y1)+pow2(x2-x1))<pow2(r);
}

struct Enemy{

	explicit Enemy( int type, int posx, int posy )
		:type( type ),posx( posx ), posy( posy )
		,tek( miki::make_image(   "src/enemy_run.png",6,6,1,16,16 ) )
		,atk( miki::make_image("src/enemy_attack.png",6,6,1,16,16 ) )
		,fly( miki::make_image("src/enemy2_run.png") )
		,fly_dead( miki::make_image("src/enemy2_damage.png",4,4,1,16,16) )
		,usha( miki::make_image("src/u_run.png",6,6,1,32,32 ) )
		,usha_j( miki::make_image("src/u_jump.png") )
		,yobu( miki::make_image("src/enemy3.png",6,6,1,16,16 ) )
		,usha_jp( miki::make_image("src/u_n_jump.png") )
		,usha_atk( miki::make_image("src/u_attack.png",10,10,1,128,128) )
		,u_yarare( miki::make_image("src/u_damage.png") )
		,cnt( 0 ),hp(5)
		,isdie( false )
	{}

	bool draw( int camx, int camy ){

		switch ( type ){
		case 0: // ÇƒÇ≠ÇƒÇ≠
			posx -= 2;
			DrawGraph( posx - camx, posy - camy, miki::to_draw( tek, (++g_index/4)%6 ), TRUE );
			if ( (GetRand(240) < 5) | (posx < 220) ){
				g_index = 0;
				type = 2;
			}
			return false;//posx < 100;
		case 1: // ÇµÇ⁄Å[ÇÒ
			if ( cnt == 0 ){
				static const auto au = miki::make_sound("src/ta_ta_warekie02.mp3");
				PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
			}
			movy += (cnt%4==0);
			posx += movx; posy += movy;
			DrawRotaGraph( posx, posy, 1.0, 0.1*cnt*(movx+0.1), miki::to_draw( tek ), TRUE );
			return ++cnt > 300;
		case 2: // ÇÕÇ¡ÇµÇ·Å[
			if ( (( int(++g_index)-30)/8) == 6 ){
				if ( posx > 100+250 ){
					type = 0;
					return false;
				}else{
					g_index = 0;
				}
			}
			DrawGraph( posx - camx, posy - camy, miki::to_draw( atk, std::max( (int(g_index)-30)/8, 0 ) ), TRUE );
			if ( g_index == 30+40 ){
				Enemy tmp( 3, posx+16, posy+16 );

				tmp.movx = -(100+posx)/70;
				tmp.movy = -6-GetRand(4);
				add_enemies.push_back( tmp );

				static const auto au = miki::make_sound("src/sen_ge_gun_syouon01.mp3");
				if ( PlayerHp > 0 )
				PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
			}
			return false;
		case 3: // Ç‰Ç›Ç‚ÅI
			movy += (cnt%6==0);
			posx += movx; posy += movy;

			DrawLine( posx + (movx), posy + (movy), posx, posy, 0 );
			if ( (isin(170,200,posx)&isin( 30, 200, posy )) ){
				static const auto au = miki::make_sound("src/sen_ka_ya01.mp3");
				if ( PlayerHp > 0 )
					PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );

				
				--PlayerHp;
				return true;
			}
			return (++cnt > 300);
		case 4: // îÚçs
			posx += movx;
			if ( (posx < 100+150) & (movx<0) ) movx *= -1;
			if ( (posx > 640- 50) & (movx>0) ) movx *= -1;

			if ( (movx < 0)&(GetRand(500)<3) ){
				Enemy tmp( 3, posx+16, posy+16 );

				tmp.movx = -(100+posx)/60;
				tmp.movy = GetRand(1)-1;//-6-GetRand(4);
				add_enemies.push_back( tmp );

				static const auto au = miki::make_sound("src/sen_ge_gun_syouon01.mp3");
				if ( PlayerHp > 0 )
					PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
			}

			DrawRotaGraph( posx, posy, 1.0, 0.0, miki::to_draw( fly ), TRUE, movx > 0 );
			return false;
		case 5: // îÚçséÄñS
			if ( cnt == 0 ){
				static const auto au = miki::make_sound("src/ta_ta_warekie02.mp3");
				PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
			}
			movy += (cnt%3==0);
			posx += movx; posy += movy;
			DrawRotaGraph( posx, posy, 1.0, std::atan2( double(movy),double(movx) )-3.1415*0.5, miki::to_draw( fly_dead, cnt/4%4 ), TRUE );
			return ++cnt > 300;
		case 6: // åƒÇ—çûÇ›
			if ( posx > 500 ){
				posx -= 2;
				DrawGraph( posx - camx, posy - camy, miki::to_draw( tek, (++g_index/4)%6 ), TRUE );
				cnt = 0;
			}else{
				if ( ++cnt > 4*6*3 ){
					if ( GetRand(20) < 4 )
						add_enemies.push_back( Enemy( 0, 640, 208 ) );
					if ( GetRand(20) < 6 ){
						Enemy tmp( 4, 640, GetRand(160-40)+30 );
						tmp.movx = -1 -GetRand(1);
						add_enemies.push_back( tmp );
					}
				}
				DrawGraph( posx - camx, posy - camy, miki::to_draw( yobu, (cnt/4)%6), TRUE );
				if ( cnt == 4*6*8 ){
					type = 0;
				}

			}
			return false;
		case 7: // óEé“
			posx -= 3;
			DrawGraph( posx - camx, posy - camy, miki::to_draw( usha, (++g_index/4)%6 ), TRUE );
			if ( ( posx < 380 ) ){
				movx = -8;
				movy = -5;
				type = 9;
				cnt  = 0;
			}
			if ((GetRand(500) < 5)){
				movx = -2;
				movy = - 8;
				type = 8;
				cnt = 0;
			}
			return false;//posx < 100;
		case 8: // óEé“Ç™âÒì]ÇµÇ‹Ç∑
			if ( ++cnt%3 == 0 ) ++movy;
			posx += movx;
			if ( posx < 220 ){
				movx = 8; movy = -2;
				PlayerHp -= 5;
			}
			if ( (posy+=movy)>208-16 ){
				posy = 208-16;
				type = 7;
			}
			DrawRotaGraph( posx, posy, 1.0, -cnt*0.5317, miki::to_draw( usha_j ), TRUE ); 
			return false;
		case 9: // óEé“íµñÙ
			if ( ++cnt%5 == 0 ) ++movy;
			posx += movx;
			if ( posx < 230 ){
				cnt = 0;
				type = 10;
			}
			if ( (posy+=movy)>208-16 ){
				posy = 208-16;
				type = 7;
			}
			DrawRotaGraph( posx, posy, 1.0, 0.0, miki::to_draw( usha_jp ), TRUE ); 
			return false;
		case 10: // óEé“ÇÃçUåÇÅI
			DrawRotaGraph( posx, posy, 1.0, 0.0, miki::to_draw( usha_atk, cnt/3 ), TRUE );
			if ( ( PlayerHp > 0 )&(cnt == 3*5) ){
				static const auto au = miki::make_sound("src/sen_ka_hamono_syupin01.mp3");
				PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
			}
			if ( cnt == 3*7 ){
				PlayerHp -= 10;
			}
			if ( ++cnt/3 == 10 ){
				type = 8;
				movx = 8;
				movy = -4;
			}
			return false;
		case 11: // óEé“Ç‚ÇÁÇÍ
			if ( cnt == 0 ){
				static const auto au = miki::make_sound("src/ta_ta_warekie02.mp3");
				PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
			}
			if ( ++cnt%6 == 0 ) ++movy;
			posx += movx;
			if ( (hp>=0)&((posy+=movy)>208-16) ){
				posy = 208-16;
				type = 7;
			}
			DrawRotaGraph( posx, posy, 1.0, 0.0, miki::to_draw( u_yarare ), TRUE ); 
			return posy > 240;
		}

		return false;
	}
	int type,cnt,hp;
	int posx, posy;
	int movx, movy;
	size_t g_index;
	bool isdie;
	miki::image tek,atk,fly,fly_dead,yobu,usha,usha_j,usha_jp,usha_atk,u_yarare;
};

struct Effect;
std::vector<Effect> effects;

struct Effect{
	explicit Effect( int type, int delay )
		:type( type )
		,cnt ( 0 ),delay( delay )
		,beam_e( miki::make_image("src/player_beam_effect.png", 6, 6, 1, 256, 256 ) )
		,bomb_e( miki::make_image(   "src/player_beam_bom.png", 6, 6, 1, 256, 256 ) )
		,tamashi_e( miki::make_image("src/soul.png",4,4,1,32,32) )
	{}

	bool draw( int const camx, int const camy ){
		switch ( type ){
		case 0:
			DrawGraph( 100 - camx, -20 -camy, miki::to_draw( beam_e, cnt>>2 ), TRUE );
			return (++cnt)>>2 >= 6;
		case 1:
			if ( ++cnt < delay ) return false;
			{
			double const pi = 3.141592; 
			int const posx = 150.0*std::cos( -pi*3/5 + pi*(delay-60)/30 ) + 50;
			if ( cnt == delay ){
				static const auto au = miki::make_sound("src/sen_mi_lasergun04.mp3");
				PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );

				auto const enfnder = [=]( Enemy const& e ){
					return ((e.type==0) | (e.type==2) ) &  isin( posx+128-32,posx+128+32,e.posx ); //(e.posx > posx+128-32) & (e.posx < posx+128+32);
				};
				for ( auto it = boost::find_if( enemies, enfnder )
					; it != enemies.end()
					; it = std::find_if( std::next(it), enemies.end(), enfnder ) )
				{
					auto& e = *it;
					e.type = 1; e.cnt = 0;
					e.movy = - 6 - GetRand(5);
					e.movx = GetRand(8) - 4;
					++EnCount;
				}
			}
			if ( (cnt-delay)>>2 >= 6 ) return true;
			DrawGraph(  posx - camx, 0 -camy, miki::to_draw( bomb_e, (cnt-delay)>>2 ), TRUE );
			}
			return false;
		case 2:
			{
				int const X = 134 + 100;
				int const Y = 74 - 20;
				DrawCircle( X, Y, cnt*6*3, 0, 0 );
				DrawCircle( X, Y, cnt*6*3+1, 0, 0 );
				DrawCircle( X, Y, cnt*6*3+2, 0, 0 );
				auto const enfnder = [=]( Enemy const& e ){
					return ((e.type==4)|(e.type==3)|isin(7,10,e.type)) & isin( X, Y, e.posx, e.posy, cnt*6*3 );
				};
				for ( auto it = boost::find_if( enemies, enfnder )
					; it != enemies.end()
					; it = std::find_if( std::next(it), enemies.end(), enfnder ) )
				{
					auto& e = *it;
					if ( e.type == 3 ){
						e.cnt = 99999;
					}else if ( isin(7,10,e.type) ){
						e.type = 11;
						e.cnt = 0;
						if ( --e.hp < 0 ){
							++EnCount;
						}
						e.movx = 3+GetRand(6);
						e.movy = -GetRand(4);
					}else{
						e.type = 5;
						e.cnt = 0;
						e.movx += GetRand(2)+1;
						e.movy = -2-GetRand(4);
						++EnCount;
					}
				}
			}
			return ++cnt > 6;
		case 3:
			movy += (cnt%6==0);
			posx += movx; posy += movy;

			DrawLine( posx + (movx), posy + (movy), posx, posy, 0 );

			{
				auto const enfnder = [this]( Enemy const& e ){
					return ((e.type==0)|(e.type==2)|(e.type==4)|(e.type==6)|(e.type==8)) &  isin( posx-16,posx+16,e.posx ) & isin( posy-8, posy+8, e.posy );
				};
				auto const it = boost::find_if( enemies, enfnder );
				if ( it != enemies.end() ){
					auto& e = *it;
					switch ( e.type ){
					case 0:case 2:case 6:
						e.type = 1; e.cnt = 0;
						e.movy = - 2 - GetRand(4);
						e.movx = GetRand(8) - 4;
						++EnCount;
						break;
					case 4:
						e.type = 5; e.cnt = 0;
						e.movx  += GetRand(4)-2;
						e.movy = -GetRand(3)-1;
						++EnCount;
						break;
					case 8:
						{
							Enemy tmp( 3, posx, posy );
							tmp.cnt = 0;
							tmp.movy = movy;
							tmp.movx = -(movx);
							enemies.push_back( tmp );
						}
						return true;
					}
				}
			}
			return ++cnt > 300;
		case 4:
			if (cnt%5==0) --posy;
			posx  = 170 + 30 * std::cos( 0.05 * cnt );
			DrawGraph( posx, posy, miki::to_draw( tamashi_e, cnt/4%4 ), TRUE );
			return ++cnt > 600;
		}
		return true;
	}

	int type;
	int cnt,delay;
	int movx,movy,posx,posy;
	miki::image beam_e,bomb_e,tamashi_e;
};



struct Player{

	explicit Player()
		:wait( miki::make_image("src/player_wait.png") )
		,panchi( miki::make_image("src/player_p.png", 6, 6, 1, 512, 256 ) )
		,needle( miki::make_image("src/player_needle.png", 6, 6, 1, 512, 256 ) )
		,beam  ( miki::make_image(  "src/player_beam.png", 6, 6, 1, 256, 256 ) )
		,beam_e( miki::make_image(  "src/player_beam_effect.png", 6, 6, 1, 256, 256 ) )
		,clap  ( miki::make_image(  "src/player_clap.png", 6, 6, 1, 256, 256 ) )
		,p_cnt( 0 ),n_cnt( 0 ),b_cnt( 0 ),c_cnt( 0 )
		,current_draw( miki::to_draw( wait ) )
	{}

	void draw( int const camx, int const camy ){

		if ( PlayerHp <= 0 ){

		}else if ( p_cnt ){
			++p_cnt;
			if ( p_cnt>>2 >= 6 ){
				p_cnt = 0;
			}else{
				current_draw = miki::to_draw( panchi, std::min( p_cnt>>2, 3 ) );

				if ( p_cnt == 2*4 ){
					static const auto au = miki::make_sound("src/sen_ge_sgun01.mp3");
					PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );

					auto const enfnder = [=]( Enemy const& e ){
						return ((e.type==1)|(e.type==3)|(e.type==4)|(e.type==5)|isin(7,10,e.type)) & isin( 100+82, 100+325-16, e.posx ) & isin( -20+64, -20+163, e.posy );
					};
					for ( auto it = boost::find_if( enemies, enfnder )
						; it != enemies.end()
						; it = std::find_if( std::next(it), enemies.end(), enfnder ) )
					{
						auto& e = *it;
						switch( e.type ){
						case 1:
							e.type = 1; e.cnt = 0;
							e.posx += 128;
							e.movy -= GetRand(3)+1;
							e.movx  = GetRand(6) + 8;
							break;
						case 3:
							e.cnt = 99999;
							{
								Effect tmp( 3, 0 );
								tmp.cnt = 0;
								tmp.posx = e.posx + 68;
								tmp.posy = e.posy;
								tmp.movy = 0;
								tmp.movx = -(e.movx);
								effects.push_back( tmp );
							}
							break;
						case 4:
							e.type = 5; e.cnt = 0;
							e.movx += GetRand(4);
							e.movy = -2-GetRand(4);
							++EnCount;
							break;
						case 5:
							//e.posx += 100;//340 + GetRand(30);
							e.movy  =-GetRand(3)-4;
							e.movx  = GetRand(6)+6;
							break;
						default:
							e.cnt = 0;
							e.type = 11;
							e.posx += 64;
							if ( --e.hp < 0 ){
								++EnCount;
							}
							e.movx = 3;
							e.movy = -GetRand(3)-4;
							break;
						}
					}
						//++EnCount;
				}
			}
		}else if ( n_cnt ){
			++n_cnt;
			if ( n_cnt>>2 >= 6 ){
				n_cnt = 0;
			}else{
				current_draw = miki::to_draw( needle, n_cnt>>2 );
				if ( n_cnt == 4*3 ){
					static const auto au = miki::make_sound("src/sen_ka_daiken_utiai01.mp3");
					PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
				}
				if ( n_cnt == 4*5 ){

					auto const enfnder = [=]( Enemy const& e ){
						return ((e.type==1)|(e.type==3)|(e.type==4)|(e.type==5)|isin(7,10,e.type)) & isin( 100+140, 100+392, e.posx ) & isin( -20+54, -20+192, e.posy );
					};
					for ( auto it = boost::find_if( enemies, enfnder )
						; it != enemies.end()
						; it = std::find_if( std::next(it), enemies.end(), enfnder ) )
					{
						auto& e = *it;
						switch( e.type ){
						case 1:
							e.type = 1; e.cnt = 0;
							e.movy -= GetRand(3)+1;
							e.movx  = GetRand(6) + 8;
							break;
						case 3:
							e.cnt = 99999;
							{
								Effect tmp( 3, 0 );
								tmp.cnt = 0;
								tmp.posy = e.posy;
								tmp.movy = 0;
								tmp.movx = -(e.movx*3/5);
								effects.push_back( tmp );
							}
							break;
						case 4:
							e.type = 5; e.cnt = 0;
							e.movx += GetRand(4);
							e.movy = -2-GetRand(4);
							++EnCount;
							break;
						case 5:
							e.movy  =-GetRand(3)-4;
							e.movx  = GetRand(6)+6;
							break;
						default:
							e.type = 11;
							e.cnt = 0;
							if ( --e.hp < 0 ){
								++EnCount;
							}
							e.movx = 3 + GetRand(3);
							e.movy = -GetRand(3)-4;
							break;
						}
						//++EnCount;
					}
				}
			}
		}else if ( b_cnt ){
			++b_cnt;
			if ( b_cnt>>2 >= 6 ){
				b_cnt = 0;
			}else{
				current_draw = miki::to_draw( beam, b_cnt>>2 );
			}
		}else if ( c_cnt ){
			++c_cnt;
			if ( c_cnt>>2 >= 6 ){
				c_cnt = 0;
			}else{
				current_draw = miki::to_draw( clap, c_cnt>>2 );
				if ( (c_cnt == 4*3)/* |(c_cnt == 4*3+1) */ ){
					static const auto au = miki::make_sound("src/sen_ge_hasai02.mp3");
						PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );
					effects.push_back( Effect( 2, 0 ) );
				}
			}
		}else{
			if ( KeyBuf[KEY_INPUT_Z] ){
				p_cnt = 1;
			}else if ( KeyBuf[KEY_INPUT_X] ){
				n_cnt = 1;
			}else if ( KeyBuf[KEY_INPUT_C] ){
				static const auto au = miki::make_sound("src/sen_mi_lasergun01.mp3");
					PlaySoundMem( miki::to_play(au), DX_PLAYTYPE_BACK );

				b_cnt = 1;
				effects.push_back( Effect( 0, 0 ) );
				for ( int i = 0; i < 10; ++i ){
					effects.push_back( Effect( 1, i*3+60 ) );
				}
			}else if ( KeyBuf[KEY_INPUT_V] ){
					c_cnt = 1;
			}
		}
		static int oldHP = PlayerHp;
		DrawGraph( 100 - camx - 4*( oldHP != PlayerHp ) , -20 -camy, current_draw, TRUE );
		if ( (oldHP > 0) & (PlayerHp <= 0) ){
			Effect ef( 4, 0 );
			ef.posy = 50;
			effects.push_back( ef );
		}
		oldHP = PlayerHp;
	}

	int p_cnt,n_cnt,b_cnt,c_cnt;
	miki::image wait,panchi,needle,beam,beam_e,clap;
	int current_draw;
};


inline bool IsProcessLoopGood(){
    return ( ProcessMessage()  == 0 )
		 & ( GetHitKeyStateAll( KeyBuf ) == 0 )
		 ;//& ( KeyBuf[KEY_INPUT_ESCAPE] == 0 );
}

int main(int argc,char *argv[] )
{

	SetOutApplicationLogValidFlag(FALSE);
	SetMainWindowText("BigMan");
	ChangeWindowMode( TRUE );
	SetGraphMode( 640, 240, 16 );
	if( DxLib_Init() == -1 || SetDrawScreen( DX_SCREEN_BACK )!=0 ) return -1;//èâä˙âªÇ∆ó†âÊñ âª
	SetTransColor( 255, 0, 255 );

	// WAIT
	SetWaitVSyncFlag( FALSE );

	////////////
	auto const bg = miki::make_image( "src/bg.png" );
	Player player;
	////////////
	int gamecnt = 0,cnt = 0;
	auto const hisou = miki::make_sound("src/hisou.ogg");
	PlaySoundMem(miki::to_play(hisou),DX_PLAYTYPE_LOOP);
    while( IsProcessLoopGood() ){//ÉÅÉCÉìÉãÅ[Év
		{
			int c = 0;
			switch ( (++gamecnt%=(255*2*3))/255/2 ){
			case 0:
				c = GetColor( 255 - gamecnt/4, 255 - gamecnt/4, 255 );
				break;
			case 1:
				c = GetColor( 128+(gamecnt-255*2)/4 , 128, 255-(gamecnt-255*2)/4 );
				break;
			case 2:
				c = GetColor( 255 , 128 + (gamecnt-255*4)/4, 128 + (gamecnt-255*4)/4 );
				break;
			/*case 3:
				c = GetColor( 128 - (gamecnt-255*3)/2, 0, (gamecnt-255*3)/2 );
				break;
			case 4:
				c = GetColor( ( gamecnt-255*4 ), ( gamecnt-255*4 ), 128 + ( gamecnt-255*4 )/2 );
				break;*/
			}
			DrawBox( 0, 0, 640, 240, c, TRUE );
		}//*/

		DrawGraph( 0, 0, miki::to_draw( bg ), TRUE );
		player.draw( 0, 0 );

		{
			++cnt;
			
			if ( cnt%(1800) == 1700 )
			{
				enemies.push_back( Enemy( 6, 640, 208 ) );
			}
			if ( ( EnCount > 500 )&(cnt%1800 == 1799) ){
				Enemy tmp( 9, 640, 208-16 );
				tmp.movx = -3;
				tmp.movy = -4;

				enemies.push_back( tmp );
			}
			if ( !isin( 1340, 1800, cnt%1800 ) ){
			if ( GetRand(1000) < cnt/200+2 )
			{
				enemies.push_back( Enemy( 0, 640, 208 ) );
			}
			if ( GetRand(1000) < cnt/200+2 )
			{
				Enemy tmp( 4, 640, GetRand(160-40)+30 );
				tmp.movx = -1 -GetRand(1);
				enemies.push_back( tmp );
			}
			}
		}

		boost::copy( add_enemies, std::back_inserter( enemies ) );
		add_enemies.clear();

		boost::remove_erase_if( enemies,
			[]( Enemy& e ){ return e.draw( 0, 0 ); } );
		boost::remove_erase_if( effects,
			[]( Effect& e ){ return e.draw( 0, 0 ); } );

		// HP guage
		{
			static int c = -1;
			c = -1 - c;
			DrawBox( 0, 0, PlayerHp, 10, c, TRUE );
		}

		DrawFormatString( 480 - 1, 2 - 1, 0, "Killed : %06u", EnCount );
		DrawFormatString( 480 + 1, 2 + 1, 0, "Killed : %06u", EnCount );
		DrawFormatString( 480 - 1, 2 + 1, 0, "Killed : %06u", EnCount );
		DrawFormatString( 480 + 1, 2 - 1, 0, "Killed : %06u", EnCount );
		DrawFormatString( 480, 2, -1, "Killed : %06u", EnCount );
		static int are = 255 + 400;
		if ( PlayerHp < 0 ) --are;

		if ( are < 255 ){
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255-are );
			DrawBox( 0, 0, 640, 240, -1, TRUE );
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
		ScreenFlip();
		if ( are == 0 ) break;
    }
	int aaacnt = 255;
	while ( IsProcessLoopGood() ){
		DrawBox( 0, 0, 640, 240, -1, TRUE );
		{
			static const auto ede = miki::make_image("src/bg_ed.png");
			DrawGraph( 0, 0, miki::to_draw( ede ), TRUE );
		}
		player.draw( 0, 0 );
		DrawFormatString( 30+1, 90+1, 0, "%dì˙%dî”Å@â ä∏Ç…êÌÇ¢ë±ÇØÅ@%uëÃÇ‡ÇÃìGï∫Çì|ÇµÇΩãêêlÅ@Ç±Ç±Ç…ñ∞ÇÈ", cnt/(255*6),cnt/(255*6), EnCount );
		DrawFormatString( 30-1, 90+1, 0, "%dì˙%dî”Å@â ä∏Ç…êÌÇ¢ë±ÇØÅ@%uëÃÇ‡ÇÃìGï∫Çì|ÇµÇΩãêêlÅ@Ç±Ç±Ç…ñ∞ÇÈ", cnt/(255*6),cnt/(255*6), EnCount );
		DrawFormatString( 30+1, 90-1, 0, "%dì˙%dî”Å@â ä∏Ç…êÌÇ¢ë±ÇØÅ@%uëÃÇ‡ÇÃìGï∫Çì|ÇµÇΩãêêlÅ@Ç±Ç±Ç…ñ∞ÇÈ", cnt/(255*6),cnt/(255*6), EnCount );
		DrawFormatString( 30-1, 90-1, 0, "%dì˙%dî”Å@â ä∏Ç…êÌÇ¢ë±ÇØÅ@%uëÃÇ‡ÇÃìGï∫Çì|ÇµÇΩãêêlÅ@Ç±Ç±Ç…ñ∞ÇÈ", cnt/(255*6),cnt/(255*6), EnCount );
		DrawFormatString( 30, 90, -1, "%dì˙%dî”Å@â ä∏Ç…êÌÇ¢ë±ÇØÅ@%uëÃÇ‡ÇÃìGï∫Çì|ÇµÇΩãêêlÅ@Ç±Ç±Ç…ñ∞ÇÈ", cnt/(255*6),cnt/(255*6), EnCount );

		SetDrawBlendMode( DX_BLENDMODE_ALPHA, std::max( 0, --aaacnt ) );
			DrawBox( 0, 0, 640, 240, -1, TRUE );
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );

		if ( aaacnt < -60 ){
			DrawString( 430, 150, "GAME OVER", 0 );
			DrawString( 430, 165, "PUSH ANY KEY", 0 );
			ScreenFlip();
			WaitKey();
			break;
		}
		ScreenFlip();
	}

	//ActionStage::DBG_reset();
    DxLib_End();//ÇcÇwÉâÉCÉuÉâÉäèIóπèàóù
    return 0;
}


#ifdef WIN32
int WINAPI WinMain( HINSTANCE , HINSTANCE , LPSTR , int ){
	return main( __argc, __argv );
}
#endif // ifdef WIN32