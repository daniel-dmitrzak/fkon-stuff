using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.IO.Ports;
using System;
using System.Linq; 

public class UIController : MonoBehaviour {

	private const float kRotationSpeed = 14f;
    private SerialPort sp;

	public Tesseract target;
    public GameObject cumChalice;
	public Camera camera;

	[SerializeField] private Toggle _toggleProjection;
	[SerializeField] private Toggle _toggleAutoRotate;
	[SerializeField] private Slider _sliderXY;
	[SerializeField] private Slider _sliderYZ;
	[SerializeField] private Slider _sliderZX;
	[SerializeField] private Slider _sliderXW;
	[SerializeField] private Slider _sliderYW;
	[SerializeField] private Slider _sliderZW;

    // Placeholders for target position that is controlled by the player
    private float setPositionXY;
    private float setPositionYZ;
    private float setPositionZX;
    // Placeholders for target position for animation playing 
    private float setPositionXW;
    private float setPositionYW;
    private float setPositionZW;

    public float controlSpeed = 5f;
    public bool solved = false;
    public int operationMode = 2;
    public float rotationDelay = 5;
    public float rotationTimer = 0;

    bool timerExpired = true;
    
    protected void Start()
	{
		_toggleProjection.onValueChanged.AddListener(HandleProjectionChanged);
		_toggleAutoRotate.onValueChanged.AddListener(HandleRotationChanged);

		HandleProjectionChanged(_toggleProjection.isOn);
		HandleRotationChanged(_toggleAutoRotate.isOn);

        // Handle COM port initialization 
        sp = new SerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
        sp.Open();

        if (sp.IsOpen)
        {
            Debug.Log("Serial port open!");
        }
        else
        {
            Debug.Log("Failed opening com port!");
        }

        sp.ReadTimeout = 10;
    }

	protected void Update()
	{
        if (operationMode == 0)
        {

            float amount = kRotationSpeed * Time.deltaTime;
            //target.rotationXY += amount;
            //target.rotationZX += amount;
            //target.rotationYW += amount;

            // Those will change
            //setPositionXY += amount;
            //setPositionZX += amount;
            //setPositionYW += amount;
            //// Those will assure that tesseract will not solve itsef
            //setPositionYZ = 150;
            //setPositionXW = 180;
            //setPositionZW = 120;


            //UpdateSliders();

        }
        else if (operationMode == 1)
        {
            setPositionXY = 360 * _sliderXY.value;
            setPositionYZ = 360 * _sliderYZ.value;
            setPositionZX = 360 * _sliderZX.value;
            setPositionXW = 360 * _sliderXW.value;
            setPositionYW = 360 * _sliderYW.value;
            setPositionZW = 360 * _sliderZW.value;
        }
        else if (operationMode == 2)
        {
            if (sp.IsOpen)
            {
                try
                {
                    string tempS = sp.ReadLine();
                    var parts = tempS.Split(' ');
                    int tempPos1 = Int32.Parse(parts[0]); 
                    int tempPos2 = Int32.Parse(parts[1]);

                    if(tempPos1 == 361 && tempPos2 == 361 && timerExpired && !solved)
                    {

                        float amount = kRotationSpeed * Time.deltaTime;
                        setPositionXY += amount;
                        setPositionZX += amount;
                        setPositionYW += amount;

                        setPositionYZ = 150;
                        setPositionXW = 180;
                        setPositionZW = 120;

                        UpdateSliders();

                    }
                    else if(tempPos1 == 361 && tempPos2 == 361 && !timerExpired && !solved)
                    {
                        rotationTimer += Time.deltaTime;
                        if (rotationTimer > rotationDelay) timerExpired = true;
                    }
                    else
                    {
                        if (target.rotationXY >= 342 && target.rotationXY <= 348 &&
                            target.rotationYW >= 172 && target.rotationYW <= 178) solved = true;
                        if (!solved)
                        {
                            timerExpired = false;
                            rotationTimer = 0;

                            setPositionYZ =  66;//    target.rotationYZ = setPositionYZ;
                            setPositionZX =   3;//    target.rotationZX = setPositionZX;
                            setPositionXW = 234;//    target.rotationXW = setPositionZW; 
                            setPositionZW =  28;//    target.rotationYW = setPositionYW;

                            setPositionXY = tempPos1 == 361 ? setPositionXY : tempPos1;
                            setPositionYW = tempPos2 == 361 ? setPositionYW : tempPos2;
                        }
                        else
                        {
                            //controlSpeed = 2;
                            setPositionXY = 0;
                            if (target.rotationXY < 3)
                            {
                                setPositionYZ = 0;
                                if (target.rotationYZ < 3)
                                {
                                    setPositionZX = 0;
                                    if (target.rotationZX < 3)
                                    {
                                        setPositionXW = 0;
                                        if (target.rotationXW < 3)
                                        {
                                            setPositionYW = 0;
                                            if (target.rotationXW < 3)
                                            {
                                                setPositionYW = 0;
                                                if (target.rotationYW < 3)
                                                {
                                                    setPositionZW = 0;
                                                    if (target.rotationZW < 3)
                                                    {
                                                        solved = false;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                    }


                    Debug.Log("New set position! XY:" + setPositionXY + " YW:" + setPositionYW);
                }
                catch
                {

                }
            } else
            {
                // Handle COM port initialization 
                sp = new SerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
                sp.Open();
            }
           
        }

        if (operationMode > 0)
        {
            target.rotationXY += controlSpeed * (setPositionXY - target.rotationXY) * Time.deltaTime;
            target.rotationYZ += controlSpeed * (setPositionYZ - target.rotationYZ) * Time.deltaTime;
            target.rotationZX += controlSpeed * (setPositionZX - target.rotationZX) * Time.deltaTime;
            target.rotationXW += controlSpeed * (setPositionXW - target.rotationXW) * Time.deltaTime;
            target.rotationYW += controlSpeed * (setPositionYW - target.rotationYW) * Time.deltaTime;
            target.rotationZW += controlSpeed * (setPositionZW - target.rotationZW) * Time.deltaTime;
        }
    }

    void FixedUpdate()
    {
        
    }
	private void HandleProjectionChanged(bool value)
	{
		camera.orthographic = !value;
		target.useOrthoProjection = !value;
	}

	private void HandleRotationChanged(bool value)
	{
		UpdateSliders();
	}

	private void UpdateSliders()
	{
		UpdateSlider(_sliderXY,target.rotationXY,!_toggleAutoRotate.isOn);
		UpdateSlider(_sliderYZ,target.rotationYZ,!_toggleAutoRotate.isOn);
		UpdateSlider(_sliderZX,target.rotationZX,!_toggleAutoRotate.isOn);
		UpdateSlider(_sliderXW,target.rotationXW,!_toggleAutoRotate.isOn);
		UpdateSlider(_sliderYW,target.rotationYW,!_toggleAutoRotate.isOn);
		UpdateSlider(_sliderZW,target.rotationZW,!_toggleAutoRotate.isOn);
	}

	private void UpdateSlider(Slider slider, float rotation, bool enabled)
	{
		slider.interactable = enabled;
		slider.value = (rotation%360f)/360f;
	}

    private void OnApplicationQuit()
    {
        sp.Close();
    }
}
