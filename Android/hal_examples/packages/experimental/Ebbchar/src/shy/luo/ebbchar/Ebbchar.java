package shy.luo.ebbchar;

import android.app.Activity;
import android.os.ServiceManager;
import android.os.Bundle;
import android.os.IEbbcharService;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class Ebbchar extends Activity implements OnClickListener {
	private final static String LOG_TAG = "shy.luo.ebbchar.EbbcharActivity";

	private IEbbcharService ebbcharService = null;

	private EditText valueText = null;
	private Button readButton = null;
	private Button writeButton = null;
	private Button clearButton = null;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

	ebbcharService = IEbbcharService.Stub.asInterface(
		ServiceManager.getService("ebbchar"));

        valueText = (EditText)findViewById(R.id.edit_value);
        readButton = (Button)findViewById(R.id.button_read);
        writeButton = (Button)findViewById(R.id.button_write);
        clearButton = (Button)findViewById(R.id.button_clear);

	readButton.setOnClickListener(this);
	writeButton.setOnClickListener(this);
	clearButton.setOnClickListener(this);

        Log.i(LOG_TAG, "Ebbchar Activity Created");
    }

    @Override
    public void onClick(View v) {
    	if(v.equals(readButton)) {
		try {
    			valueText.setText(ebbcharService.getOutput());
		} catch (RemoteException e) {
			Log.e(LOG_TAG, "Remote Exception while reading value from ebbchar service.");
		}
    	}
    	else if(v.equals(writeButton)) {
		try {
    			String text = valueText.getText().toString();
			ebbcharService.setInput(text);
		} catch (RemoteException e) {
			Log.e(LOG_TAG, "Remote Exception while writing value to ebbchar service.");
		}
    	}
    	else if(v.equals(clearButton)) {
    		String text = "";
    		valueText.setText(text);
    	}
    }
}
