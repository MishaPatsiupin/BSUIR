package com.example.home.ui.home;

import android.app.Application;
import androidx.annotation.NonNull;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.example.home.R;

public class HomeViewModel extends AndroidViewModel {

    private final MutableLiveData<String> mText;

    public HomeViewModel(@NonNull Application application) {
        super(application);
        mText = new MutableLiveData<>();
        mText.setValue(application.getString(R.string.home_page));
    }

    public LiveData<String> getText() {
        return mText;
    }
}