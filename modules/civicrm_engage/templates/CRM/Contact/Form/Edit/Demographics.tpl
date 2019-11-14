{*
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC. All rights reserved.                        |
 |                                                                    |
 | This work is published under the GNU AGPLv3 license with some      |
 | permitted exceptions and without any warranty. For full license    |
 | and copyright information, see https://civicrm.org/licensing       |
 +--------------------------------------------------------------------+
*}
<div class="crm-accordion-wrapper crm-demographics-accordion crm-accordion-closed">
 <div class="crm-accordion-header">
  <div class="icon crm-accordion-pointer"></div> 
	{$title} 
  </div><!-- /.crm-accordion-header -->
  <div id="demographics" class="crm-accordion-body">
  <div class="form-item">
        <span class="labels">{$form.gender_id.label}</span>
        
	<span class="fields">
        {$form.gender_id.html}
        <span class="crm-clear-link">(<a href="#" title="unselect" onclick="unselectRadio('gender_id', '{$form.formName}'); return false;">{ts}clear{/ts}</a>)</span>
        </span>
  </div>
  <div class="form-item">
        <span class="labels">{$form.birth_date.label}</span>
        <span class="fields">{include file="CRM/common/jcalendar.tpl" elementName=birth_date}</span>
  </div>
  <div class="form-item">
       {$form.is_deceased.html}
       {$form.is_deceased.label}
  </div>
  <div id="showDeceasedDate" class="form-item">
       <span class="labels">{$form.deceased_date.label}</span>
       <span class="fields">{include file="CRM/common/jcalendar.tpl" elementName=deceased_date}</span>
  </div> 
  {if isset($demographics_groupTree)}{foreach from=$demographics_groupTree item=cd_edit key=group_id}
     {foreach from=$cd_edit.fields item=element key=field_id}
        <table class="form-layout-compressed">
        {include file="CRM/Custom/Form/CustomField.tpl"}
        </table>
     {/foreach}
  {/foreach}{/if}
 </div><!-- /.crm-accordion-body -->
</div><!-- /.crm-accordion-wrapper -->

{literal}
<script type="text/javascript">
    showDeceasedDate( );    
    function showDeceasedDate( )
    {
        if (document.getElementsByName("is_deceased")[0].checked) {
      	    show('showDeceasedDate');
        } else {
	    hide('showDeceasedDate');
        }
    }     
</script>
{/literal}
