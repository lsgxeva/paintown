package com.rafkind.paintown.animator;

import java.io.File;

import java.util.Vector;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

import com.rafkind.paintown.exception.LoadException;

import com.rafkind.paintown.Lambda1;

public abstract class AnimatedObject extends BasicObject {
	
	private Vector<Animation> animations = new Vector<Animation>();
	private List updates;
    private DrawProperties drawProperties = new DrawProperties();

	public AnimatedObject(String name){
		super(name);
		updates = new ArrayList();
        this.drawProperties = drawProperties;
	}

    public DrawProperties getDrawProperties(){
        return drawProperties;
    }

	public void addAnimationUpdate( Lambda1 update ){
		updates.add(update);
	}
	
	public void removeAnimationUpdate( Lambda1 update ){
		updates.remove(update);
	}

	public void removeAnimation( int index ){
		Animation temp = (Animation)animations.elementAt(index);
		animations.removeElement(temp);
		updateAnimationListeners();
	}

	public void updateAnimationListeners(){
		for ( Iterator it = updates.iterator(); it.hasNext(); ){
			Lambda1 update = (Lambda1) it.next();
			try{
				update.invoke( this );
			} catch ( Exception e ){
				e.printStackTrace();
			}
		}
	}

	abstract public void setMap( int map );

	public void removeAnimation(Animation anim){
		removeAnimation( animations.indexOf( anim ) );
	}

	public void addAnimation( Animation a ){
		animations.add( a );
	}

	public Vector<Animation> getAnimations(){
		return animations;
	}

	public Animation getAnimation( int i ){
		return (Animation) getAnimations().get( i );
	}

}
