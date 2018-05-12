#ifndef _SCE_SAMPLE_UTIL_INPUT_CONTROLLER_H
#define _SCE_SAMPLE_UTIL_INPUT_CONTROLLER_H

#include <scebase_common.h>
#include <sampleutil/sampleutil_common.h>
#include <vectormath.h>

namespace sce
{
	namespace SampleUtil
	{
		namespace Input
		{

#if _SCE_TARGET_OS_WINDOWS || _SCE_TARGET_OS_PSP2

#if defined _SCE_TARGET_OS_PSP2 && _SCE_TARGET_OS_PSP2
			static const size_t kMaxNumControllerDataBufs = 64;
#endif //defined _SCE_TARGET_OS_PSP2 && _SCE_TARGET_OS_PSP2
#if defined _SCE_TARGET_OS_WINDOWS && _SCE_TARGET_OS_WINDOWS
			static const size_t kMaxNumControllerDataBufs = 64;
#endif //defined _SCE_TARGET_OS_WINDOWS &&  _SCE_TARGET_OS_WINDOWS

			/*!
			 * @~English
			 * 
			 * @brief Number of controller ports 
			 * @details Number of controller ports 
			 * @~Japanese
			 * 
			 * @brief コントローラーのポート数 
			 * @details コントローラーのポート数 
			 */
			static const uint32_t kControllerPortNum=4;

			/*!
			 * @~English
			 * 
			 * @brief Structure for initializing ControllerContext 
			 * @details This is the structure for initializing ControllerContext. This is used by specifying it to the argument option of ControllerContext::initialize(). 
			 * @~Japanese
			 * 
			 * @brief ControllerContextの初期化用構造体 
			 * @details ControllerContextの初期化用構造体です。ControllerContext::initialize()の引数optionに指定することで利用します。 
			 */
			struct ControllerContextOption
			{
				/*!
				 * @~English
				 * @brief Number of the buffers of the SceCtrlData structure retained within ControllerContext 
				 * @details Specify a value from 1 to 64. The default value is 64. 
				 * @~Japanese
				 * @brief ControllerContext内で保持するSceCtrlData構造体のバッファ数 
				 * @details 1～64の値を指定してください。デフォルト値は64です。 
				 */
				uint32_t	numBufs; 
				ControllerContextOption(void):
				numBufs(64)
				{
				}

			};

#endif /*_SCE_TARGET_OS_WINDOWS || _SCE_TARGET_OS_PSP2*/

			/*!
			 * @~English
			 * 
			 * @brief The enumeration type to be used for specifying a matching pattern of buttons 
			 * @details Used for specifying for the 2nd argument "pattern" of the PadContext member functions isButtonDown(), isButtonUp(), isButtonPressed(), and isButtonReleased(). 
			 * @~Japanese
			 * 
			 * @brief ボタンの一致パターンを指定するために利用する列挙型 
			 * @details PadContextのメンバ関数isButtonDown()、isButtonUp()、isButtonPressed()、isButtonReleased() の第2引数patternに指定して利用します。 
			 */
			enum ButtonEventPattern
			{
				/*!
				 * @~English
				 * @brief If any one of the specified buttons matches, the function will return true as the return value. 
				 * @~Japanese
				 * @brief 指定されたボタンがどれか一つでも一致していたら関数が返り値にtrueを返す挙動となる 
				 */
				kButtonEventPatternAny = 0,
				/*!
				 * @~English
				 * @brief If all of the specified buttons matches, the function will return true as the return value. 
				 * @~Japanese
				 * @brief 指定されたボタンが全て一致していたら関数が返り値にtrueを返す挙動となる 
				 */
				kButtonEventPatternAll
			};

			/*!
			 * @~English
			 * 
			 * @brief The enum value of a button 
			 * @details Used for specifying for the 1st argument "buttons" of the PadContext member functions isButtonDown(), isButtonUp(), isButtonPressed(), and isButtonReleased(). 
			 * @~Japanese
			 * 
			 * @brief ボタンのenum値 
			 * @details PadContextのメンバ関数isButtonDown()、isButtonUp()、isButtonPressed()、isButtonReleased()の第1引数buttonsに指定して利用します。 
			 */
			enum Button
			{
#if !_SCE_TARGET_OS_PSP2
				/*!
				 * @~English
				 * @brief L3 button 
				 * @~Japanese
				 * @brief L3ボタン 
				 */
				kButtonL3		=		(1<<1),
				/*!
				 * @~English
				 * @brief R3 button 
				 * @~Japanese
				 * @brief R3ボタン 
				 */
				kButtonR3		=		(1<<2),
#endif //!_SCE_TARGET_OS_PSP2
#if !_SCE_TARGET_OS_ORBIS
				/*!
				 * @~English
				 * @brief SELECT button 
				 * @~Japanese
				 * @brief SELECTボタン 
				 */
				kButtonSelect	=		(1<<0),
#endif // !_SCE_TARGET_OS_ORBIS

				/*!
				 * @~English
				 * @brief START button 
				 * @~Japanese
				 * @brief STARTボタン 
				 */
				kButtonStart	=		(1<<3),
#if !_SCE_TARGET_OS_PSP2
				/*!
				 * @~English
				 * @brief OPTIONS button 
				 * @~Japanese
				 * @brief OPTIONSボタン 
				 */
				kButtonOptions	=		(1<<3),
#endif // !_SCE_TARGET_OS_PSP2
				/*!
				 * @~English
				 * @brief Up button 
				 * @~Japanese
				 * @brief 方向キー上 
				 */
				kButtonUp		=		(1<<4),
				/*!
				 * @~English
				 * @brief Right button 
				 * @~Japanese
				 * @brief 方向キー右 
				 */
				kButtonRight	=		(1<<5),
				/*!
				 * @~English
				 * @brief Down button 
				 * @~Japanese
				 * @brief 方向キー下 
				 */
				kButtonDown		=		(1<<6),
				/*!
				 * @~English
				 * @brief Left button 
				 * @~Japanese
				 * @brief 方向キー左 
				 */
				kButtonLeft		=		(1<<7),
#if _SCE_TARGET_OS_PSP2
				/*!
				 * @~English
				 * @brief L button 
				 * @~Japanese
				 * @brief Lボタン 
				 */
				kButtonL		=		(1<<8),
				/*!
				 * @~English
				 * @brief R button 
				 * @~Japanese
				 * @brief Rボタン 
				 */
				kButtonR		=		(1<<9),
#else	// _SCE_TARGET_OS_PSP2

				/*!
				 * @~English
				 * @brief L2 button 
				 * @~Japanese
				 * @brief L2ボタン 
				 */
				kButtonL2		=		(1<<8),
				/*!
				 * @~English
				 * @brief R2 button 
				 * @~Japanese
				 * @brief R2ボタン 
				 */
				kButtonR2		=		(1<<9),
				/*!
				 * @~English
				 * @brief L1 button 
				 * @~Japanese
				 * @brief L1ボタン 
				 */
				kButtonL1		=		(1<<10),
				/*!
				 * @~English
				 * @brief R1 button 
				 * @~Japanese
				 * @brief R1ボタン 
				 */
				kButtonR1		=		(1<<11),

#endif	// _SCE_TARGET_OS_PSP2

				/*!
				 * @~English
				 * @brief Triangle button 
				 * @~Japanese
				 * @brief △ボタン 
				 */
				kButtonTriangle	=		(1<<12),
				/*!
				 * @~English
				 * @brief Circle button 
				 * @~Japanese
				 * @brief ○ボタン 
				 */
				kButtonCircle	=		(1<<13),
				/*!
				 * @~English
				 * @brief Cross button 
				 * @~Japanese
				 * @brief ×ボタン 
				 */
				kButtonCross	=		(1<<14),
				/*!
				 * @~English
				 * @brief Square button 
				 * @~Japanese
				 * @brief □ボタン 
				 */
				kButtonSquare	=		(1<<15)
#if !_SCE_TARGET_OS_PSP2
				,
				/*!
				 * @~English
				 * @brief Touchpad 
				 * @~Japanese
				 * @brief タッチパッド 
				 */
				kButtonTouchPad	=		(1<<20),
				/*!
				 * @~English
				 * @brief None
				 * @~Japanese
				 * @brief なし
				 */
				kButtonNone =           (1<<21)
#endif //!_SCE_TARGET_OS_PSP2
			};


			struct ControllerData 
			{
				uint64_t	timeStamp;	/*E time stamp (usec)            */
				uint32_t	buttons;	/*E pressed digital button       */
				uint8_t		lx;			/*E L analog controller (X-axis) */
				uint8_t		ly;			/*E L analog controller (Y-axis) */
				uint8_t		rx;			/*E R analog controller (X-axis) */
				uint8_t		ry;			/*E R analog controller (Y-axis) */
			};


#if _SCE_TARGET_OS_WINDOWS || _SCE_TARGET_OS_PSP2

			/*!
			 * @~English
			 * 
			 * @brief Class for handling controller operation 
			 * @details ControllerContext is a class that enable applications to easily support controller operation. The following features are provided. - Management of the button state of the controller and analog stick value- Management of the dead zones of the analog stick- Indication of the "Press" and "Release" events of a button 
			 * @~Japanese
			 * 
			 * @brief コントローラー操作を扱うためのクラス 
			 * @details ControllerContextはアプリケーションがコントローラー操作をサポートするのを簡便に行えるようにするためのクラスです。以下の機能を備えています。- コントローラーのボタン状態やアナログスティック値の管理- アナログスティックのデッドゾーンの管理- ボタンの「Press」および「Release」イベントの表現 
			 */
			class SCE_SAMPLE_UTIL_API ControllerContext : public Resource
			{
			public:

				virtual ~ControllerContext(void){}


				/*!
				 * @~English
				 * 
				 * @brief Update ControllerContext 
				 * @details This is the update function of ControllerContext. scePadGetData() is called within this function and the data of the controller in ControllerContext is updated; thus, call this function when the data needs to be updated. 
				 * @~Japanese
				 * 
				 * @brief ControllerContextを更新する 
				 * @details ControllerContextの更新関数です。本関数内でscePadGetData()を呼び出し、ControllerContext内のコントローラーのデータが更新されていますので、データの更新が必要なタイミングで呼び出すようにしてください。 
				 */
				virtual void update(void) = 0; 



				/*!
				 * @~English
				 * 
				 * @brief Function to determine whether a button is being pressed 
				 * 
				 * @param buttons The enum value of Button used to judge the function
				 * @param pattern The matching pattern of the button. PATTERN_ALL is specified as the default parameter.
				 * 
				 * @retval true The button specified in buttons is being pressed in reference to the pattern specified by pattern
				 * @retval false The button specified in buttons is not being pressed in reference to the pattern specified by pattern
				 * @details This function refers to the matching pattern specified in the pattern argument to determine whether the button specified in the buttons argument is currently being pressed. 
				 * @~Japanese
				 * 
				 * @brief ボタンが現在押されていることを判定する関数 
				 * 
				 * @param buttons 関数の判定に利用するButtonのenum値
				 * @param pattern ボタンの一致パターン。デフォルトパラメータとしてPATTERN_ALLが指定されています。
				 * 
				 * @retval true buttonsで指定されているボタンがpatternで指定されたパターンで見たときに押されている状態である
				 * @retval false buttonsで指定されているボタンがpatternで指定されたパターンで見たときに押されている状態ではない
				 * @details 引数buttonsで指定されたボタンが引数patternで指定された一致パターンで判定した際に、現在押されているかを判定する関数です。 
				 */
				virtual bool isButtonDown(uint32_t buttons, ButtonEventPattern pattern=kButtonEventPatternAll) const=0;
				/*!
				 * @~English
				 * 
				 * @brief Function to determine that a button is not currently being pressed 
				 * 
				 * @param buttons The enum value of Button used to judge the function
				 * @param pattern The matching pattern of the button. PATTERN_ALL is specified as the default parameter.
				 * 
				 * @retval true The button specified in buttons is not being pressed in reference to the pattern specified by pattern
				 * @retval false The button specified in buttons is being pressed in reference to the pattern specified by pattern
				 * @details This function refers to the matching pattern specified in the pattern argument to determine whether the button specified in the buttons argument is currently not being pressed. 
				 * @~Japanese
				 * 
				 * @brief ボタンが現在押されていないことを判定する関数 
				 * 
				 * @param buttons 関数の判定に利用するButtonのenum値
				 * @param pattern ボタンの一致パターン。デフォルトパラメータとしてPATTERN_ALLが指定されています。
				 * 
				 * @retval true buttonsで指定されているボタンがpatternで指定されたパターンで見たときに押されていない状態である
				 * @retval false buttonsで指定されているボタンがpatternで指定されたパターンで見たときに押されていない状態ではない
				 * @details 引数buttonsで指定されたボタンが引数patternで指定された一致パターンで判定した際に、現在押されていないことをを判定する関数です。 
				 */
				virtual bool isButtonUp(uint32_t buttons, ButtonEventPattern pattern=kButtonEventPatternAll) const=0;
				/*!
				 * @~English
				 * 
				 * @brief Function to determine that a button has been pressed 
				 * 
				 * @param buttons The enum value of Button used to judge the function
				 * @param pattern The matching pattern of the button. PATTERN_ALL is specified as the default parameter.
				 * 
				 * @retval true The button specified in buttons has been pressed in reference to the pattern specified by pattern
				 * @retval false The button specified in buttons has not been pressed in reference to the pattern specified by pattern
				 * @details This function refers to the matching pattern specified in the pattern argument for the button specified in the buttons arguments and compares the controller values obtained with the last update() call and the update() call preceding it to determine whether the button has been pressed. 
				 * @~Japanese
				 * 
				 * @brief ボタンが押されたことを判定する関数 
				 * 
				 * @param buttons 関数の判定に利用するButtonのenum値
				 * @param pattern ボタンの一致パターン。デフォルトパラメータとしてPATTERN_ALLが指定されています。
				 * 
				 * @retval true buttonsで指定されているボタンがpatternで指定されたパターンで見たときに押された
				 * @retval false buttonsで指定されているボタンがpatternで指定されたパターンで見たときに押されていない
				 * @details 引数buttonsで指定されたボタンが引数patternで指定された一致パターンで判定した際に、最後と最後から一つ前のupdate()で取得されたコントローラーの値と比較して、ボタンが押されたかどうかを判定する関数です。 
				 */
				virtual bool isButtonPressed(uint32_t buttons, ButtonEventPattern pattern=kButtonEventPatternAll) const=0;
				/*!
				 * @~English
				 * 
				 * @brief Function to determine that a button has been released 
				 * 
				 * @param buttons The enum value of Button used to judge the function
				 * @param pattern The matching pattern of the button. PATTERN_ALL is specified as the default parameter.
				 * 
				 * @retval true The button specified in buttons has been released in reference to the pattern specified by pattern
				 * @retval false The button specified in buttons has not been released in reference to the pattern specified by pattern
				 * @details This function refers to the matching pattern specified in the pattern argument for the button specified in the buttons arguments and compares the controller values obtained with the last update() call and the update() call preceding it to determine whether the button has been released. 
				 * @~Japanese
				 * 
				 * @brief ボタンが離されたことを判定する関数 
				 * 
				 * @param buttons 関数の判定に利用するButtonのenum値
				 * @param pattern ボタンの一致パターン。デフォルトパラメータとしてPATTERN_ALLが指定されています。
				 * 
				 * @retval true buttonsで指定されているボタンがpatternで指定されたパターンで見たときに離された
				 * @retval false buttonsで指定されているボタンがpatternで指定されたパターンで見たときに離されていない
				 * @details 引数buttonsで指定されたボタンが引数patternで指定された一致パターンで判定した際に、最後と最後から一つ前のupdate()で取得されたコントローラーの値と比較して、ボタンが離されたかどうかを判定する関数です。 
				 */
				virtual bool isButtonReleased(uint32_t buttons, ButtonEventPattern pattern=kButtonEventPatternAll) const=0;

				/*!
				 * @~English
				 * 
				 * @brief Function to get left analog stick data 
				 * 
				 * @retval sce::Vectormath::Simd::Aos::Vector2& Returns the value of the left analog stick in the Vector2 type. In the X and Y values of Vector2, the X direction and Y direction of the left analog stick are stored in a float value, respectively.
				 * @details This is the function to get left analog stick data. In the X and Y values of Vector2 of the return value, the X direction and Y direction of the left analog stick are stored in a float value, respectively. In this function, the dead zones are also taken into account and the dead zones can be changed with setDeadZone(). 
				 * @~Japanese
				 * 
				 * @brief 左アナログスティックのデータ取得関数 
				 * 
				 * @retval sce::Vectormath::Simd::Aos::Vector2& 左アナログスティックの値をVector2型で返します。Vector2のX値に左アナログスティックのX方向がfloat値で、Vector2のY値に左アナログスティックのY方向がfloat値で格納されています。
				 * @details 左アナログスティックのデータ取得関数です。返り値のVector2のX値に左アナログスティックのX方向がfloat値で、Vector2のY値に左アナログスティックのY方向がfloat値で格納されています。また、本関数ではデッドゾーンが考慮されており、このデッドゾーンはsetDeadZone()で変更可能です。 
				 */
				virtual const sce::Vectormath::Simd::Aos::Vector2& getLeftStick(void) const=0;

				/*!
				 * @~English
				 * 
				 * @brief Function to get right analog stick data 
				 * 
				 * @retval sce::Vectormath::Simd::Aos::Vector2& Returns the value of the right analog stick in the Vector2 type. In the X and Y values of Vector2, the X direction and Y direction of the right analog stick are stored in a float value, respectively.
				 * @details This is the function to get right analog stick data. In the X and Y values of Vector2 of the return value, the X direction and Y direction of the right analog stick are stored in a float value, respectively. In this function, the dead zones are also taken into account and the dead zones can be changed with setDeadZone(). 
				 * @~Japanese
				 * 
				 * @brief 右アナログスティックのデータ取得関数 
				 * 
				 * @retval sce::Vectormath::Simd::Aos::Vector2& 右アナログスティックの値をVector2型で返します。Vector2のX値に右アナログスティックのX方向がfloat値で、Vector2のY値に右アナログスティックのY方向がfloat値で格納されています。
				 * @details 右アナログスティックのデータ取得関数です。返り値のVector2のX値に右アナログスティックのX方向がfloat値で、Vector2のY値に右アナログスティックのY方向がfloat値で格納されています。また、本関数ではデッドゾーンが考慮されており、このデッドゾーンはsetDeadZone()で変更可能です。 
				 */
				virtual const sce::Vectormath::Simd::Aos::Vector2& getRightStick(void) const=0;

				/*!
				 * @~English
				 * 
				 * @brief Function to set analog stick dead zones 
				 * 
				 * @param deadZone The dead zone value of analog stick
				 * @details This is the function to set the dead zones of analog stick. The default value is set to 32.0f/127.0f (approximately 0.2520f), and this value is derived from the dead zone of analog sticks.  For details on the dead zone, refer to the "System Software Overv" and "Controller Service Overview" documents. 
				 * @~Japanese
				 * 
				 * @brief アナログスティックのデッドゾーン設定関数 
				 * 
				 * @param deadZone アナログスティックのデッドゾーン値
				 * @details アナログスティックのデッドゾーンを設定する関数です。デフォルト値として 32.0f/127.0f (約0.2520f) が設定されており、この値はアナログスティックの不感帯から導出されています。 デッドゾーンについての詳細は「システムソフトウェア概要」および「コントローラサービス 概要」ドキュメントを参照してください。 
				 */
				virtual void setDeadZone(float deadZone)=0;

				/*!
				 * @~English
				 * 
				 * @brief Function to obtain the controller data currently held by ControllerContext 
				 * 
				 * @param port Port number
				 * @param data Array of the write destination of the ControllerData structure
				 * @param length Length of the data array
				 * @return Returns the number of written data items for success. Returns an error code for failures. 
				 * @details This function obtains the controller data array currently held by ControllerContext. This is the buffer count designated at the time of initialization for maximum array length. 
				 * @~Japanese
				 * 
				 * @brief ControllerContextが現在保持しているコントローラーデータの取得関数 
				 * 
				 * @param port ポート番号
				 * @param data ControllerData構造体の書き込み先の配列
				 * @param length dataの配列の長さ
				 * @return 成功時は書きこまれたdataの数。失敗時はエラーコード。 
				 * @details ControllerContextが現在保持しているコントローラーデータの配列を取得するための関数です。配列の長さの最大は初期化時に指定したバッファ数です。 
				 */
				virtual int getData(uint32_t port, ControllerData *data, uint32_t length) const= 0;
				int getData(ControllerData *data, uint32_t length) const
				{
					return getData(0, data, length);
				}

			};

			/*!
			 * @~English
			 * 
			 * @brief Initialization function of ControllerContext 
			 * @param outControllerContext Pointer to which the generated ControllerContext returns
			 * @param option The ControllerContexOption structure. This is initialized by the default value if NULL is specified.
			 * 
			 * @retval SCE_OK Success
			 * @details This is the initialization function of ControllerContext. Call this first when using ControllerContext. 
			 * @~Japanese
			 * 
			 * @brief ControllerContextの初期化関数 
			 * @param outControllerContext 生成されたControllerContextが返されるポインタ。
			 * @param option ControllerContexOption構造体。NULLを指定するとデフォルトの値で初期化されます。
			 * 
			 * @retval SCE_OK 成功
			 * @details ControllerContextの初期化関数です。ControllerContext利用時に最初に呼び出してください。 
			 */
			SCE_SAMPLE_UTIL_API int createControllerContext(ControllerContext **outControllerContext, const ControllerContextOption* option=NULL);

#endif /*_SCE_TARGET_OS_WINDOWS || _SCE_TARGET_OS_PSP2*/

		}
	}
}



#endif /* _SCE_SAMPLE_UTIL_INPUT_CONTROLLER_H */

